#include "base/util/test_util_prod.h"

#include <dlfcn.h>

namespace base {

bool IsGTest() {
  return dlsym(RTLD_DEFAULT, "_ZN4kudu10g_is_gtestE") != nullptr;
}

} // namespace base
