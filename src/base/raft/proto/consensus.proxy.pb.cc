// THIS FILE IS AUTOGENERATED FROM base/raft/proto/consensus.proto

#include "base/raft/proto/consensus.proxy.pb.h"

#include "base/rpc/outbound_call.h"
#include "base/net/sockaddr.h"

namespace base {
namespace consensus {

ConsensusServiceProxy::ConsensusServiceProxy(
   const std::shared_ptr< ::base::rpc::Messenger> &messenger,
   const ::base::Sockaddr &remote)
  : Proxy(messenger, remote, "base.consensus.ConsensusService") {
}

ConsensusServiceProxy::~ConsensusServiceProxy() {
}


::base::Status ConsensusServiceProxy::UpdateConsensus(const ConsensusRequestPB &req, ConsensusResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("UpdateConsensus", req, resp, controller);
}

void ConsensusServiceProxy::UpdateConsensusAsync(const ConsensusRequestPB &req,
                     ConsensusResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("UpdateConsensus", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::RequestConsensusVote(const VoteRequestPB &req, VoteResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("RequestConsensusVote", req, resp, controller);
}

void ConsensusServiceProxy::RequestConsensusVoteAsync(const VoteRequestPB &req,
                     VoteResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("RequestConsensusVote", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::ChangeConfig(const ChangeConfigRequestPB &req, ChangeConfigResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("ChangeConfig", req, resp, controller);
}

void ConsensusServiceProxy::ChangeConfigAsync(const ChangeConfigRequestPB &req,
                     ChangeConfigResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("ChangeConfig", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::GetNodeInstance(const GetNodeInstanceRequestPB &req, GetNodeInstanceResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("GetNodeInstance", req, resp, controller);
}

void ConsensusServiceProxy::GetNodeInstanceAsync(const GetNodeInstanceRequestPB &req,
                     GetNodeInstanceResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("GetNodeInstance", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::RunLeaderElection(const RunLeaderElectionRequestPB &req, RunLeaderElectionResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("RunLeaderElection", req, resp, controller);
}

void ConsensusServiceProxy::RunLeaderElectionAsync(const RunLeaderElectionRequestPB &req,
                     RunLeaderElectionResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("RunLeaderElection", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::LeaderStepDown(const LeaderStepDownRequestPB &req, LeaderStepDownResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("LeaderStepDown", req, resp, controller);
}

void ConsensusServiceProxy::LeaderStepDownAsync(const LeaderStepDownRequestPB &req,
                     LeaderStepDownResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("LeaderStepDown", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::GetLastOpId(const GetLastOpIdRequestPB &req, GetLastOpIdResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("GetLastOpId", req, resp, controller);
}

void ConsensusServiceProxy::GetLastOpIdAsync(const GetLastOpIdRequestPB &req,
                     GetLastOpIdResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("GetLastOpId", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::GetConsensusState(const GetConsensusStateRequestPB &req, GetConsensusStateResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("GetConsensusState", req, resp, controller);
}

void ConsensusServiceProxy::GetConsensusStateAsync(const GetConsensusStateRequestPB &req,
                     GetConsensusStateResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("GetConsensusState", req, resp, controller, callback);
}

::base::Status ConsensusServiceProxy::StartTabletCopy(const StartTabletCopyRequestPB &req, StartTabletCopyResponsePB *resp,
                                     ::base::rpc::RpcController *controller) {
  return SyncRequest("StartTabletCopy", req, resp, controller);
}

void ConsensusServiceProxy::StartTabletCopyAsync(const StartTabletCopyRequestPB &req,
                     StartTabletCopyResponsePB *resp, ::base::rpc::RpcController *controller,
                     const ::base::rpc::ResponseCallback &callback) {
  AsyncRequest("StartTabletCopy", req, resp, controller, callback);
}

} // namespace consensus
} // namespace base
