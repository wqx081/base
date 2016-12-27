#ifndef BASE_UTIL_MUTEX_H_
#define BASE_UTIL_MUTEX_H_

#include "base/core/gscoped_ptr.h"
#include "base/core/macros.h"

#include <glog/logging.h>

#include <memory>

namespace base {
namespace util {

class Mutex {
  friend class ConditionVariable;

 public:
  Mutex();
  ~Mutex();

  void Acquire();
  void Release();
  bool TryAcquire();

  void lock() { Acquire(); }
  void unlock() { Release(); }
  bool try_lock() { return TryAcquire(); }

  void AssertAcquired() const {}

 private:
  void* GetNativeHandler();

  class Impl;
  std::unique_ptr<Impl> impl_;
  DISALLOW_COPY_AND_ASSIGN(Mutex);
};

class MutexLock {
 public:
  struct AlreadyAcquired {};

  explicit MutexLock(Mutex& lock)
    : lock_(&lock),
      owned_(true) {
    lock_->Acquire();
  }

  MutexLock(Mutex& lock, const AlreadyAcquired&)
    : lock_(&lock),
      owned_(true) {
    lock_->AssertAcquired();
  }
  
  void Lock() {
    DCHECK(!owned_);
    lock_->Acquire();
    owned_ = true;
  }

  void Unlock() {
    DCHECK(owned_);
    lock_->AssertAcquired();
    lock_->Release();
    owned_ = false;
  }

  ~MutexLock() {
    if (owned_) {
      Unlock();
    }
  }

  bool OwnsLock() const {
    return owned_;
  }

 private:
  Mutex* lock_;
  bool owned_;
  DISALLOW_COPY_AND_ASSIGN(MutexLock);
};

} // namespace util
} // namespace base
#endif // BASE_UTIL_MUTEX_H_
