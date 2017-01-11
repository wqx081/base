#ifndef BASE_SERVER_RPC_SERVER_H_
#define BASE_SERVER_RPC_SERVER_H_

#include "base/core/gscoped_ptr.h"
#include "base/core/ref_counted.h"
#include "base/rpc/service_pool.h"
#include "base/util/status.h"

#include <memory>
#include <string>
#include <vector>

namespace base {

namespace rpc {
class AcceptorPool;
class Messenger;
class ServiceIf;
} // namespace rpc

struct RpcServerOptions {
  RpcServerOptions();

  std::string rpc_bind_addresses;
  uint32_t num_acceptors_per_address;
  uint32_t num_service_threads;
  uint16_t default_port;
  size_t service_queue_length;
};

// Rpc Service 容器
// 绑定相关的地址与端口, 并且为每一个<addr:port> 创建一个AcceptorPool
// 它自身并没有做太多的处理, 基本上都是委托给了 messenger_
class RpcServer {
 public:
  explicit RpcServer(RpcServerOptions opts);
  ~RpcServer();

  // 初始化
  Status Init(const std::shared_ptr<rpc::Messenger>& messenger);
  // 注册 service
  Status RegisterService(gscoped_ptr<rpc::ServiceIf> service);
  // 绑定地址
  Status Bind();
  // 启动 server
  Status Start();
  void Shutdown();

  std::string ToString() const;
  Status GetBoundAddresses(std::vector<Sockaddr>* addresses) const;

  const rpc::ServicePool* server_pool(const std::string& service_name) const;

 private:
  enum ServerState {
    UNINITIALIZED,
    INITIALIZED,
    BOUND,
    STARTED,
  }; 
  ServerState server_state_;

  const RpcServerOptions options_;
  std::shared_ptr<rpc::Messenger> messenger_;

  std::vector<Sockaddr> rpc_bind_addresses_;
  std::vector<std::shared_ptr<rpc::AcceptorPool>> acceptor_pools_;

  DISALLOW_COPY_AND_ASSIGN(RpcServer);
};

} // namespace base
#endif // BASE_SERVER_RPC_SERVER_H_
