#ifndef KUDU_UTIL_MALLOC_H
#define KUDU_UTIL_MALLOC_H

#include <stdint.h>

namespace base {
namespace util {

// Simple wrapper for malloc_usable_size().
//
// Really just centralizes the const_cast, as this function is often called
// on const pointers (i.e. "this" in a const method).
int64_t base_malloc_usable_size(const void* obj);

} // namespace util
} // namespace base

#endif // KUDU_UTIL_MALLOC_H
