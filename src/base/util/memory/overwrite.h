#ifndef BASE_UTIL_MEMORY_OVERWRITE_H
#define BASE_UTIL_MEMORY_OVERWRITE_H

#include "base/core/strings/stringpiece.h"

namespace base {

// Overwrite 'p' with enough repetitions of 'pattern' to fill 'len'
// bytes. This is optimized at -O3 even in debug builds, so is
// reasonably efficient to use.
void OverwriteWithPattern(char* p, size_t len, StringPiece pattern);

} // namespace base
#endif /* BASE_UTIL_MEMORY_OVERWRITE_H */
