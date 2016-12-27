#include "base/util/semaphore.h"
#include "base/core/walltime.h"

#include <semaphore.h>
#include <glog/logging.h>

namespace base {
namespace util {

class Semaphore::Impl {
 public:
  explicit Impl(int capacity) {
    DCHECK_GE(capacity, 0);
    if (::sem_init(&sem_, 0, capacity) != 0) {
      Fatal("init");
    }
  }
  ~Impl() {
    if (::sem_destroy(&sem_) != 0) {
      Fatal("sem_destroy");
    }
  }

  void Acquire() {
    while (true) {
      int ret = ::sem_wait(&sem_);
      if (ret == 0) {
        return;
      }

      if (errno == EINTR) continue;
      Fatal("wait");
    }
  }

  bool TryAcquire() {
    int ret = sem_trywait(&sem_);
    if (ret == 0) {
      return true;
    }
    if (errno == EAGAIN || errno == EINTR) {
      return false;
    }
    Fatal("trywait");
    return false;
  }

  bool TimedAcquire(const MonoDelta& timeout) {
    int64_t microtime = GetCurrentTimeMicros();
    microtime += timeout.ToMicroseconds();

    struct timespec abs_timeout;
    MonoDelta::NanosToTimeSpec(microtime * MonoTime::kNanosecondsPerMicrosecond,
                               &abs_timeout);
    while (true) {
      int ret = ::sem_timedwait(&sem_, &abs_timeout);
      if (ret == 0) return true;
      if (errno == ETIMEDOUT) return false;
      if (errno == EINTR) continue;
      Fatal("timedout");
    }    
  }

  void Release() {
    PCHECK(::sem_post(&sem_) == 0);
  }

  int GetValue() {
    int val;
    PCHECK(::sem_getvalue(&sem_, &val) == 0);
    return val;
  }

 private:

  void Fatal(const char* action) {
    PLOG(FATAL) << "Could not " << action << " semaphore " << reinterpret_cast<void*>(&sem_);
    abort();
  }

  sem_t sem_;  
};

// Semaphore
Semaphore::Semaphore(int capacity)
    : impl(new Impl(capacity)) {}

Semaphore::~Semaphore() {}

void Semaphore::Acquire() {
  impl->Acquire();
}

bool Semaphore::TimedAcquire(const MonoDelta& timeout) {
  return impl->TimedAcquire(timeout);
}

bool Semaphore::TryAcquire() {
  return impl->TryAcquire();
}

void Semaphore::Release() {
  impl->Release();
}

int Semaphore::GetValue() {
  return impl->GetValue();
}

} // namespace util
} // namespace base
