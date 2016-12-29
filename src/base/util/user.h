#ifndef BASE_UTIL_USER_H
#define BASE_UTIL_USER_H

#include <string>

#include "base/util/status.h"

namespace base {

// Get current logged-in user with getpwuid_r().
// user name is written to user_name.
Status GetLoggedInUser(std::string* user_name);

} // namespace base

#endif // BASE_UTIL_USER_H
