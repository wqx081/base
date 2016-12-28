#include "base/util/condition_variable.h"

#include <pthread.h>
#include <glog/logging.h>
#include <errno.h>

namespace base {

class ConditionVariable::Impl {
 public:
  explicit Impl(Mutex* user_lock)
      : user_mutex_(static_cast<pthread_mutex_t*>(user_lock->GetNativeHandler())) {
    int rv = 0;
    pthread_condattr_t attrs;
    rv = pthread_condattr_init(&attrs);    
    DCHECK_EQ(0, rv);
    ::pthread_condattr_setclock(&attrs, CLOCK_MONOTONIC);
    rv = ::pthread_cond_init(&condition_, &attrs);
    pthread_condattr_destroy(&attrs);
    DCHECK_EQ(0, rv);
  }
        
  ~Impl() {
    int rv = ::pthread_cond_destroy(&condition_);
    DCHECK_EQ(0, rv);
  }

  void Wait() const {
    int rv = ::pthread_cond_wait(&condition_, user_mutex_);
    DCHECK_EQ(0, rv);
  }

  bool TimedWait(const MonoDelta& max_time) const {
    int64 nsecs = max_time.ToNanoseconds();
    if (nsecs < 0) {
      return false;
    }

    struct timespec relative_time;
    max_time.ToTimeSpec(&relative_time);  
    struct timespec absolute_time;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    absolute_time.tv_sec = now.tv_sec;
    absolute_time.tv_nsec = now.tv_nsec;

    absolute_time.tv_sec += relative_time.tv_sec;
    absolute_time.tv_nsec += relative_time.tv_nsec;
    absolute_time.tv_sec += absolute_time.tv_nsec / MonoTime::kNanosecondsPerSecond;
    absolute_time.tv_nsec %= MonoTime::kNanosecondsPerSecond;
    DCHECK_GE(absolute_time.tv_sec, now.tv_sec);  // Overflow paranoia
    int rv = pthread_cond_timedwait(&condition_, user_mutex_, &absolute_time);
    DCHECK(rv == 0 || rv == ETIMEDOUT)
               << "unexpected pthread_cond_timedwait return value: " << rv;
    return rv == 0;
  }

  void Broadcast() {
    int rv = ::pthread_cond_broadcast(&condition_);
    DCHECK_EQ(0, rv);
  }

  void Signal() {
    int rv = ::pthread_cond_signal(&condition_);
    DCHECK_EQ(0, rv);
  }

 private:
  mutable pthread_cond_t condition_;  
  pthread_mutex_t* user_mutex_;
};

// ConditionVariable
ConditionVariable::ConditionVariable(Mutex* user_lock)
    : impl_(new Impl(user_lock)) {}

ConditionVariable::~ConditionVariable() {}

void ConditionVariable::Wait() const {
  impl_->Wait();
}

bool ConditionVariable::TimedWait(const MonoDelta& max_time) const {
  return impl_->TimedWait(max_time);
}

void ConditionVariable::Broadcast() {
  impl_->Broadcast();
}

void ConditionVariable::Signal() {
  impl_->Signal();
}

} // namespace base
