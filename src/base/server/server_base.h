#ifndef BASE_SERVER_SERVER_BASE_H_
#define BASE_SERVER_SERVER_BASE_H_

#include "base/server/server_base_options.h"
#include "base/core/gscoped_ptr.h"
#include "base/core/macros.h"
#include "base/core/ref_counted.h"
#include "base/rpc/service_if.h"
#include "base/util/status.h"

#include <memory>
#include <string>

namespace base {

class Env;
class MetricEntity;
class MetricRegistry;
class RpcServer;
class Sockaddr;
class Thread;

namespace rpc {
class Messenger;
class ServiceIf;
} // namespace rpc

namespace server {

// 为所有server 的基类
// 处理启动, 停止 RPC server 以及 web server, 以及一些相关的度量
// 
// 对RPC的处理:
//   ServerBase 负责维护 Messenger 类
//   RpcServer  负责维护 AccpetorPool 类
//
class ServerBase {
 public:
  const RpcServer* rpc_server() const;
  const std::shared_ptr<rpc::Messenger>& messenger() const;

  Sockaddr first_rpc_address() const;
  const scoped_refptr<MetricEntity>& metric_entity() const;
  MetricEntity* metric_registry();

  const scoped_refptr<rpc::ResultTracker>& result_tracker() const; 

 protected:
  ServerBase(std::string name, const ServerBaseOptions);
  virtual ~ServerBase();

  Status Init();
  Status RegisterService(gscoped_ptr<rpc::ServiceIf> rpc_impl);
  Status Start();
  void Shutdown();

  const std::string name_;

  gscoped_ptr<MetricRegistry> metric_registry_;
  scoped_refptr<MetricEntity> metric_entity_;
  gscoped_ptr<RpcServer> rpc_server_;
  std::shared_ptr<rpc::Messenger> messenger_;
  scoped_refptr<rpc::ResultTracker> result_tracker_;
  bool is_first_run_;


 private:
  ServerBaseOptions options_;

  DISALLOW_COPY_AND_ASSIGN(ServerBase);
};

} // namespace server
} // namespace base
#endif // BASE_SERVER_SERVER_BASE_H_
