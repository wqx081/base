#include "base/util/mutex.h"

#include "base/core/walltime.h"

#include <pthread.h>

namespace base {

class Mutex::Impl {
 public:
  Impl() {
    ::pthread_mutex_init(&native_handler_, nullptr);
  }

  ~Impl() {
    int rv = ::pthread_mutex_destroy(&native_handler_);
    DCHECK_EQ(0, rv) << ". " << strerror(rv);
  }

  void Acquire() {
    if (PREDICT_TRUE(TryAcquire())) {
      return;
    }
    int rv = ::pthread_mutex_lock(&native_handler_);
    DCHECK_EQ(0, rv) << ". " << strerror(rv);
  }

  void Release() {
    int rv = ::pthread_mutex_unlock(&native_handler_);
    DCHECK_EQ(0, rv) << ". " << strerror(rv);
  }

  bool TryAcquire() {
    int rv = ::pthread_mutex_trylock(&native_handler_);
    return rv == 0;
  }

  void* GetNativeHandler() {
    return &native_handler_;
  }

 private:
  pthread_mutex_t native_handler_;
};

// Mutex
Mutex::Mutex()
    : impl_(new Impl()) {
}

Mutex::~Mutex() {
}

void Mutex::Acquire() {
  impl_->Acquire();
}

void Mutex::Release() {
  impl_->Release();
}

bool Mutex::TryAcquire() {
  return impl_->TryAcquire();
}

void* Mutex::GetNativeHandler() {
  return impl_->GetNativeHandler();
}

} // namespace base
