#include "base/util/locks.h"
#include "base/util/malloc.h"

namespace base {

size_t percpu_rwlock::memory_footprint_excluding_this() const {
  return n_cpus_ * sizeof(padded_lock);
}

size_t percpu_rwlock::memory_footprint_including_this() const {
  return base_malloc_usable_size(this) + memory_footprint_excluding_this();
}

} // namespace base
