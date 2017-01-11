#ifndef KUDU_CONSENSUS_LOG_METRICS_H
#define KUDU_CONSENSUS_LOG_METRICS_H

#include "base/core/macros.h"
#include "base/core/ref_counted.h"
#include "base/util/monotime.h"

namespace base {

class Counter;
class Histogram;
class MetricEntity;

namespace log {

struct LogMetrics {
  explicit LogMetrics(const scoped_refptr<MetricEntity>& metric_entity);

  // Global stats
  scoped_refptr<Counter> bytes_logged;

  // Per-group group commit stats
  scoped_refptr<Histogram> sync_latency;
  scoped_refptr<Histogram> append_latency;
  scoped_refptr<Histogram> group_commit_latency;
  scoped_refptr<Histogram> roll_latency;
  scoped_refptr<Histogram> entry_batches_per_group;
};

} // namespace log
} // namespace base

#endif // KUDU_CONSENSUS_LOG_METRICS_H
