#ifndef KUDU_UTIL_CACHE_METRICS_H
#define KUDU_UTIL_CACHE_METRICS_H

#include <stdint.h>

#include "base/core/macros.h"
#include "base/core/ref_counted.h"

namespace base {

template<class T>
class AtomicGauge;
class Counter;
class MetricEntity;

struct CacheMetrics {
  explicit CacheMetrics(const scoped_refptr<MetricEntity>& metric_entity);

  scoped_refptr<Counter> inserts;
  scoped_refptr<Counter> lookups;
  scoped_refptr<Counter> evictions;
  scoped_refptr<Counter> cache_hits;
  scoped_refptr<Counter> cache_hits_caching;
  scoped_refptr<Counter> cache_misses;
  scoped_refptr<Counter> cache_misses_caching;

  scoped_refptr<AtomicGauge<uint64_t> > cache_usage;
};

} // namespace base
#endif /* KUDU_UTIL_CACHE_METRICS_H */
