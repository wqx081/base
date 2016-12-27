#ifndef BASE_UTIL_COUNTDOWN_LATCH_H_
#define BASE_UTIL_COUNTDOWN_LATCH_H_

#include "base/core/macros.h"
#include "base/util/condition_variable.h"
#include "base/util/monotime.h"
#include "base/util/mutex.h"

namespace base {
namespace util {

class CountDownLatch {
 public:
  explicit CountDownLatch(int count)
    : cond_(&lock_),
      count_(count) {
  }

  void CountDown(int amount) {
    DCHECK_GE(amount, 0);
    MutexLock lock(lock_);
    if (count_ == 0) {
      return;
    }
    if (amount >= count_) {
      count_ = 0;
    } else {
      count_ -= amount;
    }

    if (count_ == 0) {
      cond_.Broadcast();
    }
  }

  void CountDown() {
    CountDown(1);
  }

  void Wait() const {
    MutexLock lock(lock_);
    while (count_ > 0) {
      cond_.Wait();
    }
  }

  bool WaitUtil(const MonoTime& when) const {
    return WaitFor(when - MonoTime::Now());
  }

  bool WaitFor(const MonoDelta& delta) const {
    MutexLock lock(lock_);
    while (count_ > 0) {
      if (!cond_.TimedWait(delta)) {
        return false;
      }
    }
    return true;
  }

  void Reset(uint64_t count) {
    MutexLock lock(lock_);
    count_ = count;
    if (count_ == 0) {
      cond_.Broadcast();
    }
  }

  uint64_t count() const {
    MutexLock lock(lock_);
    return count_;
  }

 private:
  mutable Mutex lock_;
  ConditionVariable cond_;

  uint64_t count_;

  DISALLOW_COPY_AND_ASSIGN(CountDownLatch);
};

class CountDownOnScopeExit {
 public:
  explicit CountDownOnScopeExit(CountDownLatch* latch)
    : latch_(latch) {}

  ~CountDownOnScopeExit() {
    latch_->CountDown();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CountDownOnScopeExit);

  CountDownLatch* latch_;
};

} // namespace util
} // namespace base
#endif // BASE_UTIL_COUNTDOWN_LATCH_H_
