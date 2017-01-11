#include "base/fs/block_manager.h"

#include <mutex>

#include <glog/logging.h>

#include "base/core/integral_types.h"
#include "base/core/strings/substitute.h"
#include "base/util/env.h"
#include "base/util/metrics.h"

DEFINE_bool(block_coalesce_close, false,
            "Coalesce synchronization of data during CloseBlocks()");

DEFINE_bool(block_manager_lock_dirs, true,
            "Lock the data block directories to prevent concurrent usage. "
            "Note that read-only concurrent usage is still allowed.");

DEFINE_int64(block_manager_max_open_files, -1,
             "Maximum number of open file descriptors to be used for data "
             "blocks. If 0, there is no limit. If -1, Kudu will use half of "
             "its resource limit as per getrlimit(). This is a soft limit.");

using strings::Substitute;

namespace base {
namespace fs {

BlockManagerOptions::BlockManagerOptions()
  : read_only(false) {
}

BlockManagerOptions::~BlockManagerOptions() {
}

int64_t GetFileCacheCapacityForBlockManager(Env* env) {
  // Maximize this process' open file limit first, if possible.
  static std::once_flag once;
  std::call_once(once, [&]() {
    env->IncreaseOpenFileLimit();
  });

  // See block_manager_max_open_files.
  if (FLAGS_block_manager_max_open_files == -1) {
    return env->GetOpenFileLimit() / 2;
  }
  if (FLAGS_block_manager_max_open_files == 0) {
    return kint64max;
  }
  int64_t file_limit = env->GetOpenFileLimit();
  LOG_IF(FATAL, FLAGS_block_manager_max_open_files > file_limit) <<
      Substitute(
          "Configured open file limit (block_manager_max_open_files) $0 "
          "exceeds process fd limit (ulimit) $1",
          FLAGS_block_manager_max_open_files, file_limit);
  return FLAGS_block_manager_max_open_files;
}

} // namespace fs
} // namespace base
