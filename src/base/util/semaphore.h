#ifndef BASE_UTIL_SEMAPHORE_H_
#define BASE_UTIL_SEMAPHORE_H_

#include "base/core/macros.h"
#include "base/core/port.h"
#include "base/util/monotime.h"


#include <memory>

namespace base {

// Briage Pattern
class Semaphore {
 public:
  explicit Semaphore(int capacity);
  ~Semaphore();

  void Acquire();
  bool TimedAcquire(const MonoDelta& timeout);
  bool TryAcquire();
  void Release();
  int GetValue();

  void lock() { Acquire(); }
  void unlock() { Release(); }
  bool try_lock() { return TryAcquire(); }

 private:
  class Impl;
  std::unique_ptr<Impl> impl;

  DISALLOW_COPY_AND_ASSIGN(Semaphore);
};

} // namespace base
#endif // BASE_UTIL_SEMAPHORE_H_
