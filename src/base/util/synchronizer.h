#ifndef BASE_UTIL_SYNCHRONIZER_H_
#define BASE_UTIL_SYNCHRONIZER_H_
#include "base/core/bind.h"
#include "base/core/macros.h"
#include "base/util/countdown_latch.h"

#include "base/util/status.h"
#include "base/util/status_callback.h"

namespace base {

class Synchronizer {
 public:

  Synchronizer()
    : l_(1) {
  }

  void StatusCB(const Status& status) {
    s = status;
    l_.CountDown();
  }

  StatusCallback AsStatusCallback() {
    return core::Bind(&Synchronizer::StatusCB, core::Unretained(this));
  }

  Status Wait() {
    l_.Wait();
    return s;
  }

  Status WaitFor(const MonoDelta& delta) {
    if (PREDICT_FALSE(!l_.WaitFor(delta))) {
      return Status::TimedOut("Timed out while waiting for the callback to be called.");
    }
    return s;
  }

  void Reset() {
    l_.Reset(1);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Synchronizer);
  Status s;
  CountDownLatch l_;
};

} // namespace base
#endif // BASE_UTIL_SYNCHRONIZER_H_
