#ifndef BASE_UTIL_ERRNO_H
#define BASE_UTIL_ERRNO_H

#include <string>

namespace base {

void ErrnoToCString(int err, char *buf, size_t buf_len);

// Return a string representing an errno.
inline static std::string ErrnoToString(int err) {
  char buf[512];
  ErrnoToCString(err, buf, sizeof(buf));
  return std::string(buf);
}

} // namespace base
#endif
