#ifndef BASE_AUXILIARY_SYNCHRONIZATION_PROFILING_H_
#define BASE_AUXILIARY_SYNCHRONIZATION_PROFILING_H_

#include "base/core/basictypes.h"

namespace core {

// We can do contention-profiling of SpinLocks, but the code is in
// mutex.cc, which is not always linked in with spinlock.  Hence we
// provide a weak definition, which are used if mutex.cc isn't linked in.

// Submit the number of cycles the spinlock spent contending.
ATTRIBUTE_WEAK extern void SubmitSpinLockProfileData(const void *, int64);
extern void SubmitSpinLockProfileData(const void *contendedlock,
                                      int64 wait_cycles) {}
}
#endif  // BASE_AUXILIARY_SYNCHRONIZATION_PROFILING_H_
