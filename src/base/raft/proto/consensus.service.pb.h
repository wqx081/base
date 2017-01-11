// THIS FILE IS AUTOGENERATED FROM base/raft/proto/consensus.proto

#ifndef BASE_RPC_CONSENSUS_SERVICE_IF_DOT_H
#define BASE_RPC_CONSENSUS_SERVICE_IF_DOT_H

#include "base/raft/proto/consensus.pb.h"

#include <functional>
#include <memory>
#include <string>

#include "base/rpc/rpc_header.pb.h"
#include "base/rpc/service_if.h"

namespace base {
class MetricEntity;
namespace rpc {
class Messenger;
class ResultTracker;
class RpcContext;
} // namespace rpc
} // namespace base

namespace base {
namespace consensus {

class ConsensusServiceIf : public ::base::rpc::GeneratedServiceIf {
 public:
  explicit ConsensusServiceIf(const scoped_refptr<::base::MetricEntity>& entity, const scoped_refptr<::base::rpc::ResultTracker>& result_tracker);
  virtual ~ConsensusServiceIf();
  std::string service_name() const override;
  static std::string static_service_name();

  virtual void UpdateConsensus(const ConsensusRequestPB *req,
     ConsensusResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void RequestConsensusVote(const VoteRequestPB *req,
     VoteResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void ChangeConfig(const ChangeConfigRequestPB *req,
     ChangeConfigResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void GetNodeInstance(const GetNodeInstanceRequestPB *req,
     GetNodeInstanceResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void RunLeaderElection(const RunLeaderElectionRequestPB *req,
     RunLeaderElectionResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void LeaderStepDown(const LeaderStepDownRequestPB *req,
     LeaderStepDownResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void GetLastOpId(const GetLastOpIdRequestPB *req,
     GetLastOpIdResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void GetConsensusState(const GetConsensusStateRequestPB *req,
     GetConsensusStateResponsePB *resp, ::base::rpc::RpcContext *context) = 0;
  virtual void StartTabletCopy(const StartTabletCopyRequestPB *req,
     StartTabletCopyResponsePB *resp, ::base::rpc::RpcContext *context) = 0;

};

} // namespace consensus
} // namespace base

#endif
