#include "base/util/malloc.h"

#include <malloc.h>

namespace base {

int64_t base_malloc_usable_size(const void* obj) {
  return malloc_usable_size(const_cast<void*>(obj));
}

} // namespace base
