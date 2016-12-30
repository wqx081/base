// Imported from Impala. Changes include:
// - Namespace + imports.
// - Fixes for cpplint.
// - Fixed parsing when thread names have spaces.

#ifndef BASE_UTIL_OS_UTIL_H
#define BASE_UTIL_OS_UTIL_H

#include <string>

#include "base/util/status.h"

namespace base {

// Utility methods to read interesting values from /proc.
// TODO: Get stats for parent process.

// Container struct for statistics read from the /proc filesystem for a thread.
struct ThreadStats {
  int64_t user_ns;
  int64_t kernel_ns;
  int64_t iowait_ns;

  // Default constructor zeroes all members in case structure can't be filled by
  // GetThreadStats.
  ThreadStats() : user_ns(0), kernel_ns(0), iowait_ns(0) { }
};

// Populates ThreadStats object using a given buffer. The buffer is expected to
// conform to /proc/<pid>/task/<tid>/stat layout; an error will be returned otherwise.
//
// If 'name' is supplied, the extracted thread name will be written to it.
Status ParseStat(const std::string&buffer, std::string* name, ThreadStats* stats);

// Populates ThreadStats object for a given thread by reading from
// /proc/<pid>/task/<tid>/stat. Returns OK unless the file cannot be read or is in an
// unrecognised format, or if the kernel version is not modern enough.
Status GetThreadStats(int64_t tid, ThreadStats* stats);

// Disable core dumps for this process.
//
// This is useful particularly in tests where we have injected failures and don't
// want to generate a core dump from an "expected" crash.
void DisableCoreDumps();

} // namespace base

#endif /* BASE_UTIL_OS_UTIL_H */
