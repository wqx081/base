#include "base/tests/rpc/rpct.pb.h"
#include "base/tests/rpc/rpct.service.pb.h"
#include "base/tests/rpc/rpct.proxy.pb.h"

#include "base/core/walltime.h"
#include "base/core/strings/substitute.h"

#include "base/util/test_util.h"
#include "base/util/monotime.h"
#include "base/util/env.h"
#include "base/util/random.h"
#include "base/util/random_util.h"

#include "base/rpc/acceptor_pool.h"
#include "base/rpc/messenger.h"
#include "base/rpc/proxy.h"
#include "base/rpc/reactor.h"
#include "base/rpc/remote_method.h"
#include "base/rpc/result_tracker.h"
#include "base/rpc/rpc_context.h"
#include "base/rpc/rpc_controller.h"
#include "base/rpc/rpc_sidecar.h"
#include "base/rpc/service_if.h"
#include "base/rpc/service_pool.h"
#include "base/rpc/inbound_call.h"
#include "base/rpc/serialization.h"

#include <algorithm>
#include <atomic>
#include <list>
#include <memory>
#include <string>


namespace base {
namespace rpc {

using base::rpct::AddRequestPB;
using base::rpct::AddRequestPartialPB;
using base::rpct::AddResponsePB;


class GenericCalculatorServiceImpl : public ServiceIf {
 public:
  static const char* kFullServiceName;
  static const char* kAddMethodName;
  
  GenericCalculatorServiceImpl() {}

  explicit GenericCalculatorServiceImpl(const scoped_refptr<MetricEntity>& entity,
                                        const scoped_refptr<ResultTracker>& result_tracker) {
  }

  virtual ~GenericCalculatorServiceImpl() override {
  }

  // Dispatcher
  virtual void Handle(InboundCall* incoming) override {
    if (incoming->remote_method().method_name() == kAddMethodName) {
      DoAdd(incoming);
    } else {
      incoming->RespondFailure(ErrorStatusPB::ERROR_NO_SUCH_METHOD,
                               Status::InvalidArgument("bad method"));
    }  
  }

  virtual std::string service_name() const override {
    return kFullServiceName;
  }

  static std::string static_service_name() {
    return kFullServiceName;
  }

 private:

  void DoAdd(InboundCall* incoming) {
    Slice param(incoming->serialized_request());
    AddRequestPB req;
    if (!req.ParseFromArray(param.data(), param.size())) {
      LOG(FATAL) << "Couldn't parse: " << param.ToDebugString();
    }
    AddResponsePB resp;
    resp.set_result(req.x() + req.y());
    incoming->RespondSuccess(resp);
  }  

};

const char* GenericCalculatorServiceImpl::kFullServiceName = "base.rpc.GenericCalculatorServiceImpl";
const char* GenericCalculatorServiceImpl::kAddMethodName = "Add";

class RpctTestBase : public MprTest {
 public:
  RpctTestBase()
    : n_worker_threads_(2),
      service_queue_length_(100),
      n_server_reactor_threads_(2),
      keepalive_time_ms_(1000),
      metric_entity_(METRIC_ENTITY_server.Instantiate(&metric_registry_, "test.rpct_test")) {}

  virtual void SetUp() override {
    MprTest::SetUp();
  }

  virtual void TearDown() override {
    if (service_pool_) {
      server_messenger_->UnregisterService(service_name_);
      service_pool_->Shutdown();
    }
    if (server_messenger_) {
      server_messenger_->Shutdown();
    }
    MprTest::TearDown();
  }

 protected:
  
  // 创建通用 Messnger, 该 Messenger 可用于Server/Client.
  // 对 Messenger 进行简单的配置.
  std::shared_ptr<Messenger> CreateMessenger(const std::string& name,
                                             int n_reactors = 1,
                                             bool enable_ssl = false) {
    DVLOG(6) << "Start CreateMessenger";
    // TODO(wqx):
    // Not handle enable_ssl yet

    MessengerBuilder builder(name);

    // 设置用来执行Sending/Receiving 操作的 Reactor Thread个数.
    builder.set_num_reactors(n_reactors);

    // 设置空闲TCP connection 时长
    builder.set_connection_keepalive_time(
      MonoDelta::FromMilliseconds(keepalive_time_ms_));

    // 为了保证 keepalive 正确, 我们需要比keepalive时长更小的一个频率扫描
    // 所有的连接.
    builder.set_coarse_timer_granularity(
      MonoDelta::FromMilliseconds(std::min(keepalive_time_ms_ / 5, 100)));
    builder.set_metric_entity(metric_entity_);

    std::shared_ptr<Messenger> result;
    CHECK_OK(builder.Build(&result));
    DVLOG(6) << "Finished CreateMessenger";

    return result;
  }

  Status DoTestSyncCall(const Proxy& proxy, const char* method) {
    DVLOG(6) << "Start DoTestSyncCall";

    AddRequestPB req;
    req.set_x(rand());
    req.set_y(rand());

    AddResponsePB resp;
    RpcController controller;
    controller.set_timeout(MonoDelta::FromMilliseconds(10 * 1000)); // 10s
    RETURN_NOT_OK(proxy.SyncRequest(method, req, &resp, &controller));
    
    CHECK_EQ(req.x() + req.y(), resp.result());

    DVLOG(6) << "Finished DoTestSyncCall";
    return Status::OK();
  }

  void StartTestServer(Sockaddr* server_addr, bool enable_ssl = false) {
    DoStartTestServer<GenericCalculatorServiceImpl>(server_addr, enable_ssl);
  }

 private:

  template<typename ServiceClass>
  void DoStartTestServer(Sockaddr *server_addr, bool enable_ssl = false) {

    DVLOG(6) << "Start DoStartTestServer";
    // 创建 Messenger
    server_messenger_ = CreateMessenger("TestServer",
                                        n_server_reactor_threads_,
                                        enable_ssl);  

    // 由于 server_messenger_ 用于服务端, 所以必须设这 AcceptorPool, 
    // 用于 Accept 新的连接.
    std::shared_ptr<AcceptorPool> pool;
    ASSERT_OK(server_messenger_->AddAcceptorPool(Sockaddr(), &pool));
    ASSERT_OK(pool->Start(2));

    *server_addr = pool->bind_address();
    mem_tracker_ = MemTracker::CreateTracker(-1, "result_tracker");
    result_tracker_.reset(new ResultTracker(mem_tracker_)); 

    // ServiceIf 为每一个 Service的基类
    gscoped_ptr<ServiceIf> service(new ServiceClass(metric_entity_, result_tracker_));
    service_name_ = service->service_name();
    scoped_refptr<MetricEntity> metric_entity = server_messenger_->metric_entity();

    // 设置 ServicePool
    service_pool_ = new ServicePool(std::move(service),
                                    metric_entity,
                                    service_queue_length_);

    // 注册 ServicePool
    server_messenger_->RegisterService(service_name_, service_pool_);

    // 启动 ServicePool
    // TODO(wqx):
    // Init -> Strart
    ASSERT_OK(service_pool_->Init(n_worker_threads_));
    DVLOG(6) << "Finished DoStartTestServer";
  }

 protected:
  std::string service_name_;
  std::shared_ptr<Messenger> server_messenger_;
  scoped_refptr<ServicePool> service_pool_;
  std::shared_ptr<base::MemTracker> mem_tracker_;
  scoped_refptr<ResultTracker> result_tracker_;
  int n_worker_threads_;
  int service_queue_length_;
  int n_server_reactor_threads_;
  int keepalive_time_ms_;

  MetricRegistry metric_registry_;
  scoped_refptr<MetricEntity> metric_entity_;
};

//
class TestRpct : public RpctTestBase,
                 public ::testing::WithParamInterface<bool> {
};

INSTANTIATE_TEST_CASE_P(OptionalSSL, TestRpct, testing::Values(false, true));

#if 0
TEST_F(TestRpct, TestSockaddr) {
  Sockaddr addr1, addr2;
  addr1.set_port(1000);
  addr2.set_port(2000);

  ASSERT_FALSE(addr1 < addr2);
  ASSERT_FALSE(addr2 < addr1);
  ASSERT_EQ(1000, addr1.port());
  ASSERT_EQ(2000, addr2.port());
  ASSERT_EQ(std::string("0.0.0.0:1000"), addr1.ToString());
  ASSERT_EQ(std::string("0.0.0.0:2000"), addr2.ToString());
  Sockaddr addr3(addr1);
  ASSERT_EQ(std::string("0.0.0.0:1000"), addr3.ToString());
}

TEST_P(TestRpct, TestMessengerCreateAndDestory) {
  std::shared_ptr<Messenger> messenger(CreateMessenger("TestCreateAndDestory",
                                                       1,
                                                       GetParam()));
  LOG(INFO) << "Started Messnger " << messenger->name();
  messenger->Shutdown();
}

TEST_P(TestRpct, TestAcceptorPoolStartStop) {
  int n_iters = 100;
  for (int i = 0; i < n_iters; ++i) {
    std::shared_ptr<Messenger> messenger(CreateMessenger("TestAcceptorPoolStartStop",
                                                         1,
                                                         GetParam()));
    std::shared_ptr<AcceptorPool> pool;
    ASSERT_OK(messenger->AddAcceptorPool(Sockaddr(), &pool));
    Sockaddr bound_addr;
    ASSERT_OK(pool->GetBoundAddress(&bound_addr));
    ASSERT_NE(0, bound_addr.port());
    ASSERT_OK(pool->Start(2));
    messenger->Shutdown();
  }
}

TEST_F(TestRpct, TestConnHeaderValidation) {
  MessengerBuilder builder("TestRpct.TestConnHeaderValidation");
  const int conn_hdr_len = kMagicNumberLength + kHeaderFlagsLength;
  uint8_t buf[conn_hdr_len];
  serialization::SerializeConnHeader(buf);
  ASSERT_OK(serialization::ValidateConnHeader(Slice(buf, conn_hdr_len)));
}
#endif

TEST_P(TestRpct, TestCall) {
  // Set up server
  Sockaddr server_addr;
  bool enable_ssl = GetParam();
  StartTestServer(&server_addr, enable_ssl);

  LOG(INFO) << "Connecting to " << server_addr.ToString();
  std::shared_ptr<Messenger> client_messenger(CreateMessenger("Client", 1, enable_ssl));
  Proxy p(client_messenger, server_addr, GenericCalculatorServiceImpl::static_service_name());
  ASSERT_STR_CONTAINS(p.ToString(), strings::Substitute("base.rpc.GenericCalculatorServiceImpl@"
                                                        "{remote=$0, user_credentials=",
                                                        server_addr.ToString()));

//  for (int i = 0; i < 10; i++) {
    ASSERT_OK(DoTestSyncCall(p, GenericCalculatorServiceImpl::kAddMethodName));
//  }
}


} // namespace rpc
} // namespace base

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
