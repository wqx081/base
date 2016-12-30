#ifndef BASE_RPC_RPC_TEST_BASE_H
#define BASE_RPC_RPC_TEST_BASE_H

#include <algorithm>
#include <atomic>
#include <list>
#include <memory>
#include <string>

#include "base/core/walltime.h"
#include "base/rpc/acceptor_pool.h"
#include "base/rpc/messenger.h"
#include "base/rpc/proxy.h"
#include "base/rpc/reactor.h"
#include "base/rpc/remote_method.h"
#include "base/rpc/result_tracker.h"
#include "base/rpc/rpc_context.h"
#include "base/rpc/rpc_sidecar.h"

#include "base/tests/rpc/rtest.pb.h"
#include "base/tests/rpc/rtest.proxy.pb.h"
#include "base/tests/rpc/rtest.service.pb.h"

#include "base/rpc/service_if.h"
#include "base/rpc/service_pool.h"
#include "base/util/env.h"
#include "base/util/faststring.h"
#include "base/util/mem_tracker.h"
#include "base/net/sockaddr.h"
#include "base/util/path_util.h"
#include "base/util/random.h"
#include "base/util/random_util.h"
#include "base/util/stopwatch.h"
#include "base/util/test_util.h"
#include "base/util/trace.h"

DECLARE_string(rpc_ssl_server_certificate);
DECLARE_string(rpc_ssl_private_key);
DECLARE_string(rpc_ssl_certificate_authority);

namespace base { namespace rpc {

using base::rpc_test::AddRequestPB;
using base::rpc_test::AddRequestPartialPB;
using base::rpc_test::AddResponsePB;
using base::rpc_test::CalculatorError;
using base::rpc_test::CalculatorServiceIf;
using base::rpc_test::CalculatorServiceProxy;
using base::rpc_test::EchoRequestPB;
using base::rpc_test::EchoResponsePB;
using base::rpc_test::ExactlyOnceRequestPB;
using base::rpc_test::ExactlyOnceResponsePB;
using base::rpc_test::FeatureFlags;
using base::rpc_test::PanicRequestPB;
using base::rpc_test::PanicResponsePB;
using base::rpc_test::SendTwoStringsRequestPB;
using base::rpc_test::SendTwoStringsResponsePB;
using base::rpc_test::SleepRequestPB;
using base::rpc_test::SleepResponsePB;
using base::rpc_test::TestInvalidResponseRequestPB;
using base::rpc_test::TestInvalidResponseResponsePB;
using base::rpc_test::WhoAmIRequestPB;
using base::rpc_test::WhoAmIResponsePB;
using base::rpc_test_diff_package::ReqDiffPackagePB;
using base::rpc_test_diff_package::RespDiffPackagePB;

// Implementation of CalculatorService which just implements the generic
// RPC handler (no generated code).
class GenericCalculatorService : public ServiceIf {
 public:
  static const char *kFullServiceName;
  static const char *kAddMethodName;
  static const char *kSleepMethodName;
  static const char *kSendTwoStringsMethodName;
  static const char *kAddExactlyOnce;

  static const char* kFirstString;
  static const char* kSecondString;

  GenericCalculatorService() {
  }

  // To match the argument list of the generated CalculatorService.
  explicit GenericCalculatorService(const scoped_refptr<MetricEntity>& entity,
                                    const scoped_refptr<ResultTracker>& result_tracker) {
    // this test doesn't generate metrics, so we ignore the argument.
  }

  void Handle(InboundCall *incoming) override {
    if (incoming->remote_method().method_name() == kAddMethodName) {
      DoAdd(incoming);
    } else if (incoming->remote_method().method_name() == kSleepMethodName) {
      DoSleep(incoming);
    } else if (incoming->remote_method().method_name() == kSendTwoStringsMethodName) {
      DoSendTwoStrings(incoming);
    } else {
      incoming->RespondFailure(ErrorStatusPB::ERROR_NO_SUCH_METHOD,
                               Status::InvalidArgument("bad method"));
    }
  }

  std::string service_name() const override { return kFullServiceName; }
  static std::string static_service_name() { return kFullServiceName; }

 private:
  void DoAdd(InboundCall *incoming) {
    Slice param(incoming->serialized_request());
    AddRequestPB req;
    if (!req.ParseFromArray(param.data(), param.size())) {
      LOG(FATAL) << "couldn't parse: " << param.ToDebugString();
    }

    AddResponsePB resp;
    resp.set_result(req.x() + req.y());
    incoming->RespondSuccess(resp);
  }

  void DoSendTwoStrings(InboundCall* incoming) {
    Slice param(incoming->serialized_request());
    SendTwoStringsRequestPB req;
    if (!req.ParseFromArray(param.data(), param.size())) {
      LOG(FATAL) << "couldn't parse: " << param.ToDebugString();
    }

    gscoped_ptr<faststring> first(new faststring);
    gscoped_ptr<faststring> second(new faststring);

    Random r(req.random_seed());
    first->resize(req.size1());
    RandomString(first->data(), req.size1(), &r);

    second->resize(req.size2());
    RandomString(second->data(), req.size2(), &r);

    SendTwoStringsResponsePB resp;
    int idx1, idx2;
    CHECK_OK(incoming->AddRpcSidecar(
        make_gscoped_ptr(new RpcSidecar(std::move(first))), &idx1));
    CHECK_OK(incoming->AddRpcSidecar(
        make_gscoped_ptr(new RpcSidecar(std::move(second))), &idx2));
    resp.set_sidecar1(idx1);
    resp.set_sidecar2(idx2);

    incoming->RespondSuccess(resp);
  }

  void DoSleep(InboundCall *incoming) {
    Slice param(incoming->serialized_request());
    SleepRequestPB req;
    if (!req.ParseFromArray(param.data(), param.size())) {
      incoming->RespondFailure(ErrorStatusPB::ERROR_INVALID_REQUEST,
        Status::InvalidArgument("Couldn't parse pb",
                                req.InitializationErrorString()));
      return;
    }

    LOG(INFO) << "got call: " << req.ShortDebugString();
    SleepFor(MonoDelta::FromMicroseconds(req.sleep_micros()));
    SleepResponsePB resp;
    incoming->RespondSuccess(resp);
  }
};

class CalculatorService : public CalculatorServiceIf {
 public:
  explicit CalculatorService(const scoped_refptr<MetricEntity>& entity,
                             const scoped_refptr<ResultTracker> result_tracker)
    : CalculatorServiceIf(entity, result_tracker),
      exactly_once_test_val_(0) {
  }

  void Add(const AddRequestPB *req, AddResponsePB *resp, RpcContext *context) override {
    resp->set_result(req->x() + req->y());
    context->RespondSuccess();
  }

  void Sleep(const SleepRequestPB *req, SleepResponsePB *resp, RpcContext *context) override {
    if (req->return_app_error()) {
      CalculatorError my_error;
      my_error.set_extra_error_data("some application-specific error data");
      context->RespondApplicationError(CalculatorError::app_error_ext.number(),
                                       "Got some error", my_error);
      return;
    }

    // Respond w/ error if the RPC specifies that the client deadline is set,
    // but it isn't.
    if (req->client_timeout_defined()) {
      MonoTime deadline = context->GetClientDeadline();
      if (deadline == MonoTime::Max()) {
        CalculatorError my_error;
        my_error.set_extra_error_data("Timeout not set");
        context->RespondApplicationError(CalculatorError::app_error_ext.number(),
                                        "Missing required timeout", my_error);
        return;
      }
    }

    if (req->deferred()) {
      // Spawn a new thread which does the sleep and responds later.
      scoped_refptr<Thread> thread;
      CHECK_OK(Thread::Create("rpc-test", "deferred",
                              &CalculatorService::DoSleep, this, req, context,
                              &thread));
      return;
    }
    DoSleep(req, context);
  }

  void Echo(const EchoRequestPB *req, EchoResponsePB *resp, RpcContext *context) override {
    resp->set_data(req->data());
    context->RespondSuccess();
  }

  void WhoAmI(const WhoAmIRequestPB* /*req*/,
              WhoAmIResponsePB* resp,
              RpcContext* context) override {
    const UserCredentials& creds = context->user_credentials();
    resp->mutable_credentials()->set_real_user(creds.real_user());
    resp->set_address(context->remote_address().ToString());
    context->RespondSuccess();
  }

  void TestArgumentsInDiffPackage(const ReqDiffPackagePB *req,
                                  RespDiffPackagePB *resp,
                                  ::base::rpc::RpcContext *context) override {
    context->RespondSuccess();
  }

  void Panic(const PanicRequestPB* req, PanicResponsePB* resp, RpcContext* context) override {
    TRACE("Got panic request");
    PANIC_RPC(context, "Test method panicking!");
  }

  void TestInvalidResponse(const TestInvalidResponseRequestPB* req,
                           TestInvalidResponseResponsePB* resp,
                           RpcContext* context) override {
    switch (req->error_type()) {
      case rpc_test::TestInvalidResponseRequestPB_ErrorType_MISSING_REQUIRED_FIELD:
        // Respond without setting the 'resp->response' protobuf field, which is
        // marked as required. This exercises the error path of invalid responses.
        context->RespondSuccess();
        break;
      case rpc_test::TestInvalidResponseRequestPB_ErrorType_RESPONSE_TOO_LARGE:
        resp->mutable_response()->resize(FLAGS_rpc_max_message_size + 1000);
        context->RespondSuccess();
        break;
      default:
        LOG(FATAL);
    }
  }

  bool SupportsFeature(uint32_t feature) const override {
    return feature == FeatureFlags::FOO;
  }

  void AddExactlyOnce(const ExactlyOnceRequestPB* req, ExactlyOnceResponsePB* resp,
                      ::base::rpc::RpcContext* context) override {
    if (req->sleep_for_ms() > 0) {
      usleep(req->sleep_for_ms() * 1000);
    }
    // If failures are enabled, cause them some percentage of the time.
    if (req->randomly_fail()) {
      if (rand() % 10 < 3) {
        context->RespondFailure(Status::ServiceUnavailable("Random injected failure."));
        return;
      }
    }
    int result = exactly_once_test_val_ += req->value_to_add();
    resp->set_current_val(result);
    resp->set_current_time_micros(GetCurrentTimeMicros());
    context->RespondSuccess();
  }

 private:
  void DoSleep(const SleepRequestPB *req,
               RpcContext *context) {
    TRACE_COUNTER_INCREMENT("test_sleep_us", req->sleep_micros());
    if (Trace::CurrentTrace()) {
      scoped_refptr<Trace> child_trace(new Trace());
      Trace::CurrentTrace()->AddChildTrace("test_child", child_trace.get());
      ADOPT_TRACE(child_trace.get());
      TRACE_COUNTER_INCREMENT("related_trace_metric", 1);
    }

    SleepFor(MonoDelta::FromMicroseconds(req->sleep_micros()));
    context->RespondSuccess();
  }

  std::atomic_int exactly_once_test_val_;

};

const char *GenericCalculatorService::kFullServiceName = "base.rpc.GenericCalculatorService";
const char *GenericCalculatorService::kAddMethodName = "Add";
const char *GenericCalculatorService::kSleepMethodName = "Sleep";
const char *GenericCalculatorService::kSendTwoStringsMethodName = "SendTwoStrings";
const char *GenericCalculatorService::kAddExactlyOnce = "AddExactlyOnce";

const char *GenericCalculatorService::kFirstString =
    "1111111111111111111111111111111111111111111111111111111111";
const char *GenericCalculatorService::kSecondString =
    "2222222222222222222222222222222222222222222222222222222222222222222222";

// Writes the test SSL certificate into a temporary file.
inline Status CreateSSLServerCert(const std::string& file_path) {
  static const char* test_server_cert = R"(
-----BEGIN CERTIFICATE-----
MIIEejCCA2KgAwIBAgIJAKMdvDR5PL82MA0GCSqGSIb3DQEBBQUAMIGEMQswCQYD
VQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZyYW5j
aXNjbzERMA8GA1UEChMIQ2xvdWRlcmExEjAQBgNVBAMTCWxvY2FsaG9zdDEhMB8G
CSqGSIb3DQEJARYSaGVucnlAY2xvdWRlcmEuY29tMB4XDTEzMDkyMjAwMjUxOFoX
DTQxMDIwNzAwMjUxOFowgYQxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9y
bmlhMRYwFAYDVQQHEw1TYW4gRnJhbmNpc2NvMREwDwYDVQQKEwhDbG91ZGVyYTES
MBAGA1UEAxMJbG9jYWxob3N0MSEwHwYJKoZIhvcNAQkBFhJoZW5yeUBjbG91ZGVy
YS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCoUj3pMQ2ELkrz
zq+koixljVFBAEEqwUWSjA+GEKwfFb/UPRjeO/wrKndp2r83jc6KRt66rvAIl8cr
b54yTOsJ/ZcARrjTwG3IG8Tely/54ZQyH0ImdJyEbCSoI04zX3ovjlppz3g5xanj
WmpAh6pzPgBOTfisCLMPD70xQ8F//QWZdNatoly54STkTWoJv/Oll/UpXcBY8JOR
+ytX82eGgG4F8YoQqmbjrrN5JAmqLRiBAkr3WUena6ekqJBalJRzex/Wh8a9XEV7
9HFVVngBhezsOJgf81hzBzzhULKfxuXl8uaUj3Z9cZg39CDsyz+ULYbsPm8VoMUI
VCf7MUVTAgMBAAGjgewwgekwHQYDVR0OBBYEFK94kea7jIKQawAIb+0DqsA1rf6n
MIG5BgNVHSMEgbEwga6AFK94kea7jIKQawAIb+0DqsA1rf6noYGKpIGHMIGEMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzERMA8GA1UEChMIQ2xvdWRlcmExEjAQBgNVBAMTCWxvY2FsaG9zdDEh
MB8GCSqGSIb3DQEJARYSaGVucnlAY2xvdWRlcmEuY29tggkAox28NHk8vzYwDAYD
VR0TBAUwAwEB/zANBgkqhkiG9w0BAQUFAAOCAQEAEtkPPncCnN2IFVJvz04K+VsX
b6w3qwPynQKc67+++JkNb3TYKrh/0UVM1NrEOu3TGplqOrKgAlITuaWNqNOSBu1R
WJtrz85YkonED5awjjuALVEY82+c7pOXkuv5G5421RINfRn2hNzgw8VFb5CEvxHH
jER80Vx6UGKr/S649qTQ8AzVzTwWS86VsGI2azAD7D67G/IDGf+0P7FsXonKY+vl
vKzkfaO1+qEOLtDHV9mwlsxl3Re/MNym4ExWHi9txynCNiRZHqWoZUS+KyYqIR2q
seCrQwgi1Fer9Ekd5XNjWjigC3VC3SjMqWaxeKbZ2/AuABJMz5xSiRkgwphXEQ==
-----END CERTIFICATE-----
  )";
  RETURN_NOT_OK(WriteStringToFile(Env::Default(), test_server_cert, file_path));
  return Status::OK();
}

// Writes the test SSL private key into a temporary file.
inline Status CreateSSLPrivateKey(const std::string& file_path) {
  static const char* test_private_key = R"(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAqFI96TENhC5K886vpKIsZY1RQQBBKsFFkowPhhCsHxW/1D0Y
3jv8Kyp3adq/N43Oikbeuq7wCJfHK2+eMkzrCf2XAEa408BtyBvE3pcv+eGUMh9C
JnSchGwkqCNOM196L45aac94OcWp41pqQIeqcz4ATk34rAizDw+9MUPBf/0FmXTW
raJcueEk5E1qCb/zpZf1KV3AWPCTkfsrV/NnhoBuBfGKEKpm466zeSQJqi0YgQJK
91lHp2unpKiQWpSUc3sf1ofGvVxFe/RxVVZ4AYXs7DiYH/NYcwc84VCyn8bl5fLm
lI92fXGYN/Qg7Ms/lC2G7D5vFaDFCFQn+zFFUwIDAQABAoIBABNTpiIxbLDhs998
uvQ3XsumR08kXVcwa/GgvWOSZIEJOUaAYWubDaBTNvTjlhMl6DI+YvKihZMnAkp9
fXefF1nFUWJJvI0ryi8w6RD54RtbCG4c4raRqysVU7wumZsSenAdc0o09UQE6zXc
uth/+1VSKCzVjRkLwquXDg0rD3vHfQHWQvQlzwUh3OACA3LfLezVqzrEB02YVRxm
xwg5veeMg6Aod8vsvsajry9eE0hKeFGonRANerL9lwZxzD2ZjU2fSEJYY3xxKVgi
o+QVTKaAt9pivOs10YVZDcIDH0xmDpxAkaLb5ZAbnjwhf7WGYgEm8VruAHkJxyXX
yPf3rpkCgYEA0dp/Xv5KBIkD6JJao8hnhtP5x9U7o/pTzRxaO3WUflvTI6DtC0nk
cTOwFVs4WljT76T937q2x4stuX0woHzvIaZ6wwZ2vv81ikDY6pE8eLWsH/xFAmkx
HBfkSijFgJV6EpTqUnFD7QKU89tzWrh/kxaMO1WgFaBhxPPs3K1LDTUCgYEAzVW5
3yjfVHNgjWTeAbnbToGvUihOidvIvS5cVo5q0Dhfabz0tiXFxAoQUGErUGPC8Nu2
n/HxTI3b0PbCCwjBsDNqX2kzxTSe5aTGIrBUWbped1bxme8jggXuWYbg8vvLpsYf
wAJPxnGIxW/v/aftHUhbTIuVfZX2+UnilrwiwWcCgYEAg8paz4eXaH277KVtMwq6
qZwac/tgNz0Qv/GcYVcYaLq2QNvhwoMnakhxvxfIrkS25PuTTJxwCaVIlAMhNMkB
TPrGghBfJtgUAb1z/Ow1NAG0FWpS1I7HfsMqZcBxOK2nOmA3QItNg11pujQJn+Ha
jL9OVj0SCkLs48nk6ToTtjkCgYEAh8YCtNwq6IWuN3CWGCAUMpIwIqxCWof48Zch
OZ7MZEiSVrG6QmMxpRJefTfzUyHUOj2eQZ7SxqMa0c8IuhEdOeyVjudaczD7TLAq
z68252oDovfbo8Tr/sL7OzmjryfuHqXtQqKEq5xRKvR8hYavlGhO7otx2uv5thcz
/CYE+UsCgYAsgewfzbcBqJwsAUs98DK99EU8VqKuaYlU5wNvAVb27O6sVeysSokM
G1TGIXJPphA3dSAe4Pf/j4ff/eGaS20FAFhs4BPpw0fAeCHpmD0BjIba0lxBS/gY
dc+JVPKL8Fe4a8fmsI6ndcZQ9qpOdZM5WOD0ldKRc+SsrYKkTmOOJQ==
-----END RSA PRIVATE KEY-----
  )";
  RETURN_NOT_OK(WriteStringToFile(Env::Default(), test_private_key, file_path));
  return Status::OK();
}

class RpcTestBase : public MprTest {
 public:
  RpcTestBase()
    : n_worker_threads_(3),
      service_queue_length_(100),
      n_server_reactor_threads_(3),
      keepalive_time_ms_(1000),
      metric_entity_(METRIC_ENTITY_server.Instantiate(&metric_registry_, "test.rpc_test")) {
  }

  void SetUp() override {
    MprTest::SetUp();
  }

  void TearDown() override {
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
  std::shared_ptr<Messenger> CreateMessenger(const string &name,
                                             int n_reactors = 1,
                                             bool enable_ssl = false) {
    if (enable_ssl) {
      std::string server_cert_path = JoinPathSegments(GetTestDataDirectory(), "server-cert.pem");
      std::string private_key_path = JoinPathSegments(GetTestDataDirectory(), "server-key.pem");
      CHECK_OK(CreateSSLServerCert(server_cert_path));
      CHECK_OK(CreateSSLPrivateKey(private_key_path));
      FLAGS_rpc_ssl_server_certificate = server_cert_path;
      FLAGS_rpc_ssl_private_key = private_key_path;
      FLAGS_rpc_ssl_certificate_authority = server_cert_path;
    }
    MessengerBuilder bld(name);
    bld.set_num_reactors(n_reactors);
    bld.set_connection_keepalive_time(
      MonoDelta::FromMilliseconds(keepalive_time_ms_));
    bld.set_coarse_timer_granularity(MonoDelta::FromMilliseconds(
                                       std::min(keepalive_time_ms_, 100)));
    bld.set_metric_entity(metric_entity_);
    std::shared_ptr<Messenger> messenger;
    CHECK_OK(bld.Build(&messenger));
    return messenger;
  }

  Status DoTestSyncCall(const Proxy &p, const char *method) {
    AddRequestPB req;
    req.set_x(rand());
    req.set_y(rand());
    AddResponsePB resp;
    RpcController controller;
    controller.set_timeout(MonoDelta::FromMilliseconds(10000));
    RETURN_NOT_OK(p.SyncRequest(method, req, &resp, &controller));

    CHECK_EQ(req.x() + req.y(), resp.result());
    return Status::OK();
  }

  void DoTestSidecar(const Proxy &p, int size1, int size2) {
    const uint32_t kSeed = 12345;

    SendTwoStringsRequestPB req;
    req.set_size1(size1);
    req.set_size2(size2);
    req.set_random_seed(kSeed);

    SendTwoStringsResponsePB resp;
    RpcController controller;
    controller.set_timeout(MonoDelta::FromMilliseconds(10000));
    CHECK_OK(p.SyncRequest(GenericCalculatorService::kSendTwoStringsMethodName,
                           req, &resp, &controller));

    Slice first = GetSidecarPointer(controller, resp.sidecar1(), size1);
    Slice second = GetSidecarPointer(controller, resp.sidecar2(), size2);

    Random rng(kSeed);
    faststring expected;

    expected.resize(size1);
    RandomString(expected.data(), size1, &rng);
    CHECK_EQ(0, first.compare(Slice(expected)));

    expected.resize(size2);
    RandomString(expected.data(), size2, &rng);
    CHECK_EQ(0, second.compare(Slice(expected)));
  }

  void DoTestExpectTimeout(const Proxy &p, const MonoDelta &timeout) {
    SleepRequestPB req;
    SleepResponsePB resp;
    // Sleep for 50ms longer than the call timeout.
    int sleep_micros = timeout.ToMicroseconds() + 50000;
    req.set_sleep_micros(sleep_micros);

    RpcController c;
    c.set_timeout(timeout);
    Stopwatch sw;
    sw.start();
    Status s = p.SyncRequest(GenericCalculatorService::kSleepMethodName, req, &resp, &c);
    ASSERT_FALSE(s.ok());
    sw.stop();

    int expected_millis = timeout.ToMilliseconds();
    int elapsed_millis = sw.elapsed().wall_millis();

    // We shouldn't timeout significantly faster than our configured timeout.
    EXPECT_GE(elapsed_millis, expected_millis - 10);
    // And we also shouldn't take the full time that we asked for
    EXPECT_LT(elapsed_millis * 1000, sleep_micros);
    EXPECT_TRUE(s.IsTimedOut());
    LOG(INFO) << "status: " << s.ToString() << ", seconds elapsed: " << sw.elapsed().wall_seconds();
  }

  void StartTestServer(Sockaddr *server_addr, bool enable_ssl = false) {
    DoStartTestServer<GenericCalculatorService>(server_addr, enable_ssl);
  }

  void StartTestServerWithGeneratedCode(Sockaddr *server_addr, bool enable_ssl = false) {
    DoStartTestServer<CalculatorService>(server_addr, enable_ssl);
  }

  // Start a simple socket listening on a local port, returning the address.
  // This isn't an RPC server -- just a plain socket which can be helpful for testing.
  Status StartFakeServer(Socket *listen_sock, Sockaddr *listen_addr) {
    Sockaddr bind_addr;
    bind_addr.set_port(0);
    RETURN_NOT_OK(listen_sock->Init(0));
    RETURN_NOT_OK(listen_sock->BindAndListen(bind_addr, 1));
    RETURN_NOT_OK(listen_sock->GetSocketAddress(listen_addr));
    LOG(INFO) << "Bound to: " << listen_addr->ToString();
    return Status::OK();
  }

 private:

  static Slice GetSidecarPointer(const RpcController& controller, int idx,
                                 int expected_size) {
    Slice sidecar;
    CHECK_OK(controller.GetSidecar(idx, &sidecar));
    CHECK_EQ(expected_size, sidecar.size());
    return Slice(sidecar.data(), expected_size);
  }

  template<class ServiceClass>
  void DoStartTestServer(Sockaddr *server_addr, bool enable_ssl = false) {
    server_messenger_ = CreateMessenger("TestServer", n_server_reactor_threads_, enable_ssl);
    std::shared_ptr<AcceptorPool> pool;
    ASSERT_OK(server_messenger_->AddAcceptorPool(Sockaddr(), &pool));
    ASSERT_OK(pool->Start(2));
    *server_addr = pool->bind_address();
    mem_tracker_ = MemTracker::CreateTracker(-1, "result_tracker");
    result_tracker_.reset(new ResultTracker(mem_tracker_));

    gscoped_ptr<ServiceIf> service(new ServiceClass(metric_entity_, result_tracker_));
    service_name_ = service->service_name();
    scoped_refptr<MetricEntity> metric_entity = server_messenger_->metric_entity();
    service_pool_ = new ServicePool(std::move(service), metric_entity, service_queue_length_);
    server_messenger_->RegisterService(service_name_, service_pool_);
    ASSERT_OK(service_pool_->Init(n_worker_threads_));
  }

 protected:
  string service_name_;
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

} // namespace rpc
} // namespace base
#endif