#include "base/fs/block_id.h"

#include <glog/logging.h>
#include <string>
#include <vector>

#include "base/fs/fs.pb.h"
#include "base/core/strings/join.h"

using std::string;
using std::vector;

namespace base {

const uint64_t BlockId::kInvalidId = 0;

string BlockId::JoinStrings(const vector<BlockId>& blocks) {
  vector<string> strings;
  strings.reserve(blocks.size());
  for (const BlockId& block : blocks) {
    strings.push_back(block.ToString());
  }
  return ::JoinStrings(strings, ",");
}

void BlockId::CopyToPB(BlockIdPB *pb) const {
  pb->set_id(id_);
}

BlockId BlockId::FromPB(const BlockIdPB& pb) {
  DCHECK(pb.has_id());
  return BlockId(pb.id());
}

} // namespace base
