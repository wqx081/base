#include <algorithm>
#include <atomic>
#include <memory>
#include <string>

#include "base/tests/rpc/echo.pb.h"
#include "base/tests/rpc/echo.proxy.pb.h"
#include "base/tests/rpc/echo.service.pb.h"

#include "base/core/ref_counted.h"

#include "base/rpc/inbound_call.h"
#include "base/rpc/rpc_context.h"

namespace base {
namespace rpc {

using base::echo_service::EchoRequestPB;
using base::echo_service::EchoResponsePB;

class GenericEchoServiceImpl : public ServiceIf {
 public:
  GenericEchoServiceImpl() {}
  explicit GenericEchoServiceImpl(const scoped_refptr<MetricEntity>& entity,
		                  const scoped_refptr<ResultTracker>& result) {}

  void Handle(InboundCall* incoming) override {
    if (incoming->remote_method().method_name() == "Echo") {
    
    } else {
      incoming->RespondFailure(ErrorStatusPB::ERROR_NO_SUCH_METHOD,
		               Status::InvalidArgument("bad method"));
    }
  }

  std::string service_name() const override { 
    return "base.rpc.GenericEchoServiceImpl"; 
  }

  static std::string static_service_name() {
    return "base.rpc.GenericEchoServiceImpl"; 
  }

 private:
  void DoEcho(InboundCall* incoming) {
    Slice data(incoming->serialized_request());
    EchoRequestPB req;
    if (!req.ParseFromArray(data.data(), data.size())) {
      LOG(FATAL) << "Couldn't parse: " << data.ToDebugString();
    }

    EchoResponsePB resp;
    resp.set_message(req.message());
    incoming->RespondSuccess(resp);
  }
};

class EchoServiceImpl : base::echo_service::EchoServiceIf {
 public:
  explicit EchoServiceImpl(const scoped_refptr<MetricEntity>& entity,
		           const scoped_refptr<ResultTracker> result)
	  : base::echo_service::EchoServiceIf(entity, result) {}

  void Echo(const EchoRequestPB* req, EchoResponsePB* resp, 
	    RpcContext* context) override {
    resp->set_message(req->message());
    context->RespondSuccess();
  }

 private:
  // nothing
};


} // namespace rpc
} // namespace base
