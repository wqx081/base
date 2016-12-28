#include "base/util/once.h"
#include "base/util/malloc.h"

namespace base {

size_t BaseOnceDynamic::memory_footprint_excluding_this() const {
  return status_.memory_footprint_excluding_this();
}

size_t BaseOnceDynamic::memory_footprint_including_this() const {
  return base_malloc_usable_size(this) + memory_footprint_excluding_this();
}

} // namespace base
