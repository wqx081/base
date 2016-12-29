#ifndef BASE_RPC_SERVICE_H_
#define BASE_RPC_SERVICE_H_

#include "base/core/ref_counted.h"
#include "base/util/status.h"

namespace base {
namespace rpc {

class RemoteMethod;
struct RpcMethodInfo;
class InboundCall;

class RpcService : public core::RefCountedThreadSafe<RpcService> {
 public:
  virtual ~RpcService() {}

  // Enqueue a call for processing.
  // On failure, the RpcService::QueueInboundCall() implementation is
  // responsible for responding to the client with a failure message.
  virtual Status QueueInboundCall(gscoped_ptr<InboundCall> call) = 0;

  virtual RpcMethodInfo* LookupMethod(const RemoteMethod& method) {
    return nullptr;
  }
};

} // namespace rpc
} // namespace base

#endif // BASE_RPC_SERVICE_H_
