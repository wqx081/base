#include "base/fs/block_manager_metrics.h"
#include "base/util/metrics.h"

METRIC_DEFINE_gauge_uint64(server, block_manager_blocks_open_reading,
                           "Data Blocks Open For Read",
                           base::MetricUnit::kBlocks,
                           "Number of data blocks currently open for reading");

METRIC_DEFINE_gauge_uint64(server, block_manager_blocks_open_writing,
                           "Data Blocks Open For Write",
                           base::MetricUnit::kBlocks,
                           "Number of data blocks currently open for writing");

METRIC_DEFINE_counter(server, block_manager_total_writable_blocks,
                      "Data Blocks Opened For Write",
                      base::MetricUnit::kBlocks,
                      "Number of data blocks opened for writing since service start");

METRIC_DEFINE_counter(server, block_manager_total_readable_blocks,
                      "Data Blocks Opened For Read",
                      base::MetricUnit::kBlocks,
                      "Number of data blocks opened for reading since service start");

METRIC_DEFINE_counter(server, block_manager_total_bytes_written,
                      "Block Data Bytes Written",
                      base::MetricUnit::kBytes,
                      "Number of bytes of block data written since service start");

METRIC_DEFINE_counter(server, block_manager_total_bytes_read,
                      "Block Data Bytes Read",
                      base::MetricUnit::kBytes,
                      "Number of bytes of block data read since service start");

namespace base {
namespace fs {
namespace internal {

#define MINIT(x) x(METRIC_block_manager_##x.Instantiate(entity))
#define GINIT(x) x(METRIC_block_manager_##x.Instantiate(entity, 0))
BlockManagerMetrics::BlockManagerMetrics(const scoped_refptr<MetricEntity>& entity)
  : GINIT(blocks_open_reading),
    GINIT(blocks_open_writing),
    MINIT(total_readable_blocks),
    MINIT(total_writable_blocks),
    MINIT(total_bytes_read),
    MINIT(total_bytes_written) {
}
#undef GINIT
#undef MINIT

} // namespace internal
} // namespace fs
} // namespace base
