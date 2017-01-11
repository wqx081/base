#ifndef BASE_RAFT_CONSENSUS_H_
#define BASE_RAFT_CONSENSUS_H_

#include <boost/optional/optional_fwd.hpp>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "base/raft/proto/consensus.pb.h"
#include "base/raft/ref_counted_replicate.h"

#include "base/core/callback.h"
#include "base/core/gscoped_ptr.h"
#include "base/core/ref_counted.h"

#include "base/util/status.h"
#include "base/util/status_callback.h"

namespace base {

// Forward declared
class MonoDelta;

namespace consensus {

class ConsensusCommitContinuation;
class ConsensusRound;
class ReplicaTransactionFactory;
class TimeManager;

typedef int64_t ConsensusTerm;

typedef StatusCallback ConsensusReplicatedCallback;

struct ConsensusOptions {
  std::string tablet_id;
};

struct ConsensusBootstrapInfo {
 public:
  ConsensusBootstrapInfo();
  ~ConsensusBootstrapInfo();

  OpId last_id;
  OpId last_committed_id;
  std::vector<ReplicateMsg*> orphaned_replicates;

 private:
  DISALLOW_COPY_AND_ASSIGN(ConsensusBootstrapInfo);
};

class Consensus : public core::RefCountedThreadSafe<Consensus> {
 public:
  Consensus() {}
  virtual Status Start(const ConsensusBootstrapInfo& info) = 0;
  virtual bool IsRunning() const =0;
  virtual Status EmulateElection() = 0;

  enum ElectionMode {
    NORMAL_ELECTION,
    PRE_ELECTION,
    ELECT_EVEN_IF_LEADER_IS_ALIVE
  };

  enum ElectionReason {
    INITIAL_SINGLE_NODE_ELECTION,
    ELECTION_TIMEOUT_EXPIRED,
    EXTERNAL_REQUEST
  };

  virtual Status StartElection(ElectionMode mode, ElectionReason reason) = 0;
  virtual Status WaitUntilLeaderForTests(const MonoDelta& timeout) = 0;
  virtual Status StepDown(LeaderStepDownResponsePB* resp);

  scoped_ptr<ConsensusRound> NewRound(gscoped_ptr<ReplicateMsg> replicate_msg,
                                      const ConsensusReplicatedCallback& replicated_cb);

  virtual Status Replicate(const scoped_refptr<ConsensusRound>& round) = 0;
  virtual Status CheckLeadershipAndBindTerm(const scoped_refptr<ConsensusRound>& round);

  virtual Status Update(const ConsensusRequestPB* request,
                        ConsensusResponsePB* response) = 0;
  virtual Status RequestVote(const VoteRequestPB* request,
                             VoteResponsePB* response) = 0;
  //TODO(wqx)
  virtual Status ChangeConfig(const ChangeConfigRequestPB& req,
                              const StatusCallback& client_cb);

  virtual RaftPeerPB::Role role() const = 0;
  virtual std::string peer_uuid() const = 0;
  virtual std::string tablet_id() const = 0;
  virtual scoped_refptr<TimeManager> time_manager() const = 0;
  
 protected:
 private:
};

} // namespace consensus
} // namespace base
#endif // BASE_RAFT_CONSENSUS_H_
