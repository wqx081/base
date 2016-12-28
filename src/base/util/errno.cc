#include "base/util/errno.h"

#include <errno.h>
#include <string.h>
#include <glog/logging.h>

#include "base/core/dynamic_annotations.h"

namespace base {

void ErrnoToCString(int err, char *buf, size_t buf_len) {
  CHECK_GT(buf_len, 0);
#if !defined(__GLIBC__) || \
  ((_POSIX_C_SOURCE >= 200112 || _XOPEN_SOURCE >= 600) && !defined(_GNU_SOURCE))
  // Using POSIX version 'int strerror_r(...)'.
  int ret = strerror_r(err, buf, buf_len);
  if (ret && ret != ERANGE && ret != EINVAL) {
    strncpy(buf, "unknown error", buf_len);
    buf[buf_len - 1] = '\0';
  }
#else
  // Using GLIBC version

  // KUDU-1515: TSAN in Clang 3.9 has an incorrect interceptor for strerror_r:
  // https://github.com/google/sanitizers/issues/696
  ANNOTATE_IGNORE_WRITES_BEGIN();
  char* ret = strerror_r(err, buf, buf_len);
  ANNOTATE_IGNORE_WRITES_END();
  if (ret != buf) {
    strncpy(buf, ret, buf_len);
    buf[buf_len - 1] = '\0';
  }
#endif
}

} // namespace base
