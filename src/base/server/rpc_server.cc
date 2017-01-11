#include "base/server/rpc_server.h"

#include "base/core/casts.h"
#include "base/core/gscoped_ptr.h"
#include "base/core/strings/substitute.h"

#include "base/rpc/acceptor_pool.h"
#include "base/rpc/messenger.h"
#include "base/rpc/service_if.h"
#include "base/rpc/service_pool.h"

#include "base/net/net_util.h"
#include "base/net/sockaddr.h"
#include "base/net/sockaddr.h"

#include <list>
#include <gflags/gflags.h>

using base::rpc::AcceptorPool;
using base::rpc::Messenger;
using base::rpc::ServiceIf;


DEFINE_string(rpc_bind_addresses, "0.0.0.0", "");
DEFINE_int32(rpc_num_acceptors_per_address, 1, "");
DEFINE_int32(rpc_num_service_threads, 10, "");
DEFINE_int32(rpc_service_queue_length, 50, "");
DEFINE_bool(rpc_server_allow_ephemeral_ports, false, "");

namespace base {

// 设置 messenger_
// 检查 绑定地址与端口号
Status RpcServer::Init(const std::shared_ptr<Messenger>& messenger) {
  CHECK_EQ(server_state_, UNINITIALIZED);   
  messenger_ = messenger;

  RETURN_NOT_OK(ParseAddressList(options_.rpc_bind_addresses,
                                 options_.default_port,
                                 &rpc_bind_addresses_));

  for (const Sockaddr& addr : rpc_bind_addresses_) {
    if (IsPrivilegedPort(addr.port())) {
      LOG(WARNING) << "May be unable to bind to privileged port for address "
                   << addr.ToString();
    }

    if (addr.port() == 0 && !FLAGS_rpc_server_allow_ephemeral_ports) {
      LOG(FATAL) << "Bind to ephemeral ports not supported (RPC address configured to "
                 << addr.ToString() << ")";
    }
  }

  server_state_ = INITIALIZED;
  return Status::OK();
}

// 注册 service
Status RpcServer::RegisterService(gscoped_ptr<rpc::ServiceIf> service) {
  CHECK(server_state_ == INITIALIZED ||
        server_state_ == BOUND) << "bad state: " << server_state_;

  const scoped_refptr<MetricEntity>& metric_entity = messenger_->metric_entity();
  std::string service_name = service->service_name();
  scoped_refptr<rpc::ServicePool> service_pool = 
      new rpc::ServicePool(std::move(service), metric_entity, options_.service_queue_length);

  RETURN_NOT_OK(service_pool->Init(options_.num_service_threads));
  // 实际上 service 是通过 ServicePool 注册到 Messenger 中
  RETURN_NOT_OK(messenger_->RegisterService(service_name, service_pool));
  return Status::OK();
}

// 绑定地址 以及为每一个绑定的地址添加 AcceptorPool
Status RpcServer::Bind() {
  CHECK_EQ(server_state_, INITIALIZED);

  std::vector<std::shared_ptr<AcceptorPool>> new_acceptor_pools;
  for (const Sockaddr& bind_addr : rpc_bind_addresses_) {
    std::shared_ptr<rpc::AcceptorPool> pool;
    RETURN_NOT_OK(messenger_->AddAcceptorPool(bind_addr,
                                              &pool));
    new_acceptor_pools.push_back(pool);
  }
  acceptor_pools_.swap(new_acceptor_pools);

  server_state_ = BOUND;
  return Status::OK();
}

// 启动 AcceptorPool 在绑定的地址上 accept()
Status RpcServer::Start() {
  if (server_state_ == INITIALIZED) {
    RETURN_NOT_OK(Bind());
  }
  CHECK_EQ(server_state_, BOUND);
  server_state_ = STARTED;

  for (const std::shared_ptr<AcceptorPool>& pool : acceptor_pools_) {
    // 启动 AcceptorPool 调用 accept() 返回 accpeted-socket 给 Messenger
    RETURN_NOT_OK(pool->Start(options_.num_acceptors_per_address));
  }

  std::vector<Sockaddr> bound_addrs;
  RETURN_NOT_OK(GetBoundAddresses(&bound_addrs));
  std::string bound_addrs_str;
  for (const Sockaddr& bind_addr : bound_addrs) {
    if (!bound_addrs_str.empty()) {
      bound_addrs_str += ", ";
    }
    bound_addrs_str += bind_addr.ToString();
  }
  LOG(INFO) << "RPC server started. Bound to: " << bound_addrs_str;

  return Status::OK();
}

void RpcServer::Shutdown() {
  for (const std::shared_ptr<AcceptorPool>& pool : acceptor_pools_) {
    pool->Shutdown();
  }
  acceptor_pools_.clear();

  if (messenger_) {
    WARN_NOT_OK(messenger_->UnregisterAllServices(), "Unable to unregister our services");
  }
}

Status RpcServer::GetBoundAddresses(std::vector<Sockaddr>* addresses) const {
  CHECK(server_state_ == BOUND ||
        server_state_ == STARTED) << "bad state: " << server_state_;
  for (const std::shared_ptr<AcceptorPool>& pool : acceptor_pools_) {
    Sockaddr bound_addr;
    RETURN_NOT_OK_PREPEND(pool->GetBoundAddress(&bound_addr), "Unable to get bound address from AcceptorPool");
    addresses->push_back(bound_addr);
  }
  return Status::OK();
}

const rpc::ServicePool* RpcServer::server_pool(const std::string& service_name) const {
  return down_cast<rpc::ServicePool*>(messenger_->rpc_service(service_name).get());
}

} // namespace base
