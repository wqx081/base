#ifndef KUDU_CONSENSUS_REF_COUNTED_REPLICATE_H_
#define KUDU_CONSENSUS_REF_COUNTED_REPLICATE_H_

#include "base/raft/proto/consensus.pb.h"
#include "base/core/ref_counted.h"
#include "base/core/gscoped_ptr.h"

namespace base {
namespace consensus {

// A simple ref-counted wrapper around ReplicateMsg.
class RefCountedReplicate : public core::RefCountedThreadSafe<RefCountedReplicate> {
 public:
  explicit RefCountedReplicate(ReplicateMsg* msg) : msg_(msg) {}

  ReplicateMsg* get() {
    return msg_.get();
  }

 private:
  gscoped_ptr<ReplicateMsg> msg_;
};

typedef scoped_refptr<RefCountedReplicate> ReplicateRefPtr;

inline ReplicateRefPtr make_scoped_refptr_replicate(ReplicateMsg* replicate) {
  return ReplicateRefPtr(new RefCountedReplicate(replicate));
}

} // namespace consensus
} // namespace base

#endif /* KUDU_CONSENSUS_REF_COUNTED_REPLICATE_H_ */
