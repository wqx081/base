#include "base/tests/rpc/rpc-test-base.h"

#include <string>

#include <functional>
#include <gtest/gtest.h>

#include "base/core/stl_util.h"
#include "base/core/strings/substitute.h"
#include "base/util/countdown_latch.h"
#include "base/util/metrics.h"
#include "base/util/test_util.h"

METRIC_DECLARE_counter(rpc_connections_accepted);
METRIC_DECLARE_counter(rpcs_queue_overflow);

using std::string;
using std::shared_ptr;
using strings::Substitute;

namespace base {
namespace rpc {

class MultiThreadedRpcTest : public RpcTestBase {
 public:
  // Make a single RPC call.
  void SingleCall(Sockaddr server_addr, const char* method_name,
                  Status* result, CountDownLatch* latch) {
    LOG(INFO) << "Connecting to " << server_addr.ToString();
    shared_ptr<Messenger> client_messenger(CreateMessenger("ClientSC"));
    Proxy p(client_messenger, server_addr, GenericCalculatorService::static_service_name());
    *result = DoTestSyncCall(p, method_name);
    latch->CountDown();
  }

  // Make RPC calls until we see a failure.
  void HammerServer(Sockaddr server_addr, const char* method_name,
                    Status* last_result) {
    shared_ptr<Messenger> client_messenger(CreateMessenger("ClientHS"));
    HammerServerWithMessenger(server_addr, method_name, last_result, client_messenger);
  }

  void HammerServerWithMessenger(
      Sockaddr server_addr, const char* method_name, Status* last_result,
      const shared_ptr<Messenger>& messenger) {
    LOG(INFO) << "Connecting to " << server_addr.ToString();
    Proxy p(messenger, server_addr, GenericCalculatorService::static_service_name());

    int i = 0;
    while (true) {
      i++;
      Status s = DoTestSyncCall(p, method_name);
      if (!s.ok()) {
        // Return on first failure.
        LOG(INFO) << "Call failed. Shutting down client thread. Ran " << i << " calls: "
            << s.ToString();
        *last_result = s;
        return;
      }
    }
  }
};

static void AssertShutdown(base::Thread* thread, const Status* status) {
  ASSERT_OK(ThreadJoiner(thread).warn_every_ms(500).Join());
  string msg = status->ToString();
  ASSERT_TRUE(msg.find("Service unavailable") != string::npos ||
              msg.find("Network error") != string::npos)
              << "Status is actually: " << msg;
}

// Test making several concurrent RPC calls while shutting down.
// Simply verify that we don't hit any CHECK errors.
TEST_F(MultiThreadedRpcTest, TestShutdownDuringService) {
  // Set up server.
  Sockaddr server_addr;
  StartTestServer(&server_addr);

  const int kNumThreads = 4;
  scoped_refptr<base::Thread> threads[kNumThreads];
  Status statuses[kNumThreads];
  for (int i = 0; i < kNumThreads; i++) {
    ASSERT_OK(base::Thread::Create("test", strings::Substitute("t$0", i),
      &MultiThreadedRpcTest::HammerServer, this, server_addr,
      GenericCalculatorService::kAddMethodName, &statuses[i], &threads[i]));
  }

  SleepFor(MonoDelta::FromMilliseconds(50));

  // Shut down server.
  ASSERT_OK(server_messenger_->UnregisterService(service_name_));
  service_pool_->Shutdown();
  server_messenger_->Shutdown();

  for (int i = 0; i < kNumThreads; i++) {
    AssertShutdown(threads[i].get(), &statuses[i]);
  }
}

// Test shutting down the client messenger exactly as a thread is about to start
// a new connection. This is a regression test for KUDU-104.
TEST_F(MultiThreadedRpcTest, TestShutdownClientWhileCallsPending) {
  // Set up server.
  Sockaddr server_addr;
  StartTestServer(&server_addr);

  shared_ptr<Messenger> client_messenger(CreateMessenger("Client"));

  scoped_refptr<base::Thread> thread;
  Status status;
  ASSERT_OK(base::Thread::Create("test", "test",
      &MultiThreadedRpcTest::HammerServerWithMessenger, this, server_addr,
      GenericCalculatorService::kAddMethodName, &status, client_messenger, &thread));

  // Shut down the messenger after a very brief sleep. This often will race so that the
  // call gets submitted to the messenger before shutdown, but the negotiation won't have
  // started yet. In a debug build this fails about half the time without the bug fix.
  // See KUDU-104.
  SleepFor(MonoDelta::FromMicroseconds(10));
  client_messenger->Shutdown();
  client_messenger.reset();

  ASSERT_OK(ThreadJoiner(thread.get()).warn_every_ms(500).Join());
  ASSERT_TRUE(status.IsAborted() ||
              status.IsServiceUnavailable());
  string msg = status.ToString();
  SCOPED_TRACE(msg);
  ASSERT_TRUE(msg.find("Client RPC Messenger shutting down") != string::npos ||
              msg.find("reactor is shutting down") != string::npos ||
              msg.find("Unable to start connection negotiation thread") != string::npos)
              << "Status is actually: " << msg;
}

// This bogus service pool leaves the service queue full.
class BogusServicePool : public ServicePool {
 public:
  BogusServicePool(gscoped_ptr<ServiceIf> service,
                   const scoped_refptr<MetricEntity>& metric_entity,
                   size_t service_queue_length)
    : ServicePool(std::move(service), metric_entity, service_queue_length) {
  }
  virtual Status Init(int num_threads) OVERRIDE {
    // Do nothing
    return Status::OK();
  }
};

void IncrementBackpressureOrShutdown(const Status* status, int* backpressure, int* shutdown) {
  string msg = status->ToString();
  if (msg.find("service queue is full") != string::npos) {
    ++(*backpressure);
  } else if (msg.find("shutting down") != string::npos) {
    ++(*shutdown);
  } else if (msg.find("got EOF from remote") != string::npos) {
    ++(*shutdown);
  } else {
    FAIL() << "Unexpected status message: " << msg;
  }
}

// Test that we get a Service Unavailable error when we max out the incoming RPC service queue.
TEST_F(MultiThreadedRpcTest, TestBlowOutServiceQueue) {
  const size_t kMaxConcurrency = 2;

  MessengerBuilder bld("messenger1");
  bld.set_num_reactors(kMaxConcurrency);
  bld.set_metric_entity(metric_entity_);
  CHECK_OK(bld.Build(&server_messenger_));

  shared_ptr<AcceptorPool> pool;
  ASSERT_OK(server_messenger_->AddAcceptorPool(Sockaddr(), &pool));
  ASSERT_OK(pool->Start(kMaxConcurrency));
  Sockaddr server_addr = pool->bind_address();

  gscoped_ptr<ServiceIf> service(new GenericCalculatorService());
  service_name_ = service->service_name();
  service_pool_ = new BogusServicePool(std::move(service),
                                      server_messenger_->metric_entity(),
                                      kMaxConcurrency);
  ASSERT_OK(service_pool_->Init(n_worker_threads_));
  server_messenger_->RegisterService(service_name_, service_pool_);

  scoped_refptr<base::Thread> threads[3];
  Status status[3];
  CountDownLatch latch(1);
  for (int i = 0; i < 3; i++) {
    ASSERT_OK(base::Thread::Create("test", strings::Substitute("t$0", i),
      &MultiThreadedRpcTest::SingleCall, this, server_addr,
      GenericCalculatorService::kAddMethodName, &status[i], &latch, &threads[i]));
  }

  // One should immediately fail due to backpressure. The latch is only initialized
  // to wait for the first of three threads to finish.
  latch.Wait();

  // The rest would time out after 10 sec, but we help them along.
  ASSERT_OK(server_messenger_->UnregisterService(service_name_));
  service_pool_->Shutdown();
  server_messenger_->Shutdown();

  for (const auto& thread : threads) {
    ASSERT_OK(ThreadJoiner(thread.get()).warn_every_ms(500).Join());
  }

  // Verify that one error was due to backpressure.
  int errors_backpressure = 0;
  int errors_shutdown = 0;

  for (const auto& s : status) {
    IncrementBackpressureOrShutdown(&s, &errors_backpressure, &errors_shutdown);
  }

  ASSERT_EQ(1, errors_backpressure);
  ASSERT_EQ(2, errors_shutdown);

  // Check that RPC queue overflow metric is 1
  Counter *rpcs_queue_overflow =
    METRIC_rpcs_queue_overflow.Instantiate(server_messenger_->metric_entity()).get();
  ASSERT_EQ(1, rpcs_queue_overflow->value());
}

static void HammerServerWithTCPConns(const Sockaddr& addr) {
  while (true) {
    Socket socket;
    CHECK_OK(socket.Init(0));
    Status s;
    LOG_SLOW_EXECUTION(INFO, 100, "Connect took long") {
      s = socket.Connect(addr);
    }
    if (!s.ok()) {
      CHECK(s.IsNetworkError()) << "Unexpected error: " << s.ToString();
      return;
    }
    CHECK_OK(socket.Close());
  }
}

// Regression test for KUDU-128.
// Test that shuts down the server while new TCP connections are incoming.
TEST_F(MultiThreadedRpcTest, TestShutdownWithIncomingConnections) {
  // Set up server.
  Sockaddr server_addr;
  StartTestServer(&server_addr);

  // Start a number of threads which just hammer the server with TCP connections.
  vector<scoped_refptr<base::Thread> > threads;
  for (int i = 0; i < 8; i++) {
    scoped_refptr<base::Thread> new_thread;
    CHECK_OK(base::Thread::Create("test", strings::Substitute("t$0", i),
        &HammerServerWithTCPConns, server_addr, &new_thread));
    threads.push_back(new_thread);
  }

  // Sleep until the server has started to actually accept some connections from the
  // test threads.
  scoped_refptr<Counter> conns_accepted =
    METRIC_rpc_connections_accepted.Instantiate(server_messenger_->metric_entity());
  while (conns_accepted->value() == 0) {
    SleepFor(MonoDelta::FromMicroseconds(100));
  }

  // Shutdown while there are still new connections appearing.
  ASSERT_OK(server_messenger_->UnregisterService(service_name_));
  service_pool_->Shutdown();
  server_messenger_->Shutdown();

  for (scoped_refptr<base::Thread>& t : threads) {
    ASSERT_OK(ThreadJoiner(t.get()).warn_every_ms(500).Join());
  }
}

} // namespace rpc
} // namespace base

