// Utility methods for dealing with file paths.
#ifndef BASE_UTIL_PATH_UTIL_H
#define BASE_UTIL_PATH_UTIL_H

#include <string>

namespace base {

// Common tmp infix
extern const char kTmpInfix[];
// Infix from versions of Kudu prior to 1.2.
extern const char kOldTmpInfix[];

// Join two path segments with the appropriate path separator,
// if necessary.
std::string JoinPathSegments(const std::string &a,
                             const std::string &b);

// Return the enclosing directory of path.
// This is like dirname(3) but for C++ strings.
std::string DirName(const std::string& path);

// Return the terminal component of a path.
// This is like basename(3) but for C++ strings.
std::string BaseName(const std::string& path);

} // namespace base 
#endif /* BASE_UTIL_PATH_UTIL_H */
