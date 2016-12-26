// Copyright 2008 Google Inc. All Rights Reserved.

#include <glog/logging.h>
#include "base/core/logging-inl.h"
#include "base/core/once.h"
#include "base/core/dynamic_annotations.h"
#include "base/core/spinlock_internal.h"

// All modifications to a GoogleOnceType occur inside GoogleOnceInternalInit.
// The fast path reads the variable with an acquire-load..
// This is safe provided we always perform a memory barrier
// immediately before setting the value to GOOGLE_ONCE_INTERNAL_DONE.

void GoogleOnceInternalInit(core::subtle::Atomic32 *control, void (*func)(),
                            void (*func_with_arg)(void*), void* arg) {
  if (DEBUG_MODE) {
    int32 old_control = core::subtle::Acquire_Load(control);
    if (old_control != GOOGLE_ONCE_INTERNAL_INIT &&
        old_control != GOOGLE_ONCE_INTERNAL_RUNNING &&
        old_control != GOOGLE_ONCE_INTERNAL_WAITER &&
        old_control != GOOGLE_ONCE_INTERNAL_DONE) {
      LOG(FATAL) << "Either GoogleOnceType is used in non-static storage "
                    "(where GoogleOnceDynamic might be appropriate), "
                    "or there's a memory corruption.";
    }
  }
  static const core::internal::SpinLockWaitTransition trans[] = {
    { GOOGLE_ONCE_INTERNAL_INIT, GOOGLE_ONCE_INTERNAL_RUNNING, true },
    { GOOGLE_ONCE_INTERNAL_RUNNING, GOOGLE_ONCE_INTERNAL_WAITER, false },
    { GOOGLE_ONCE_INTERNAL_DONE, GOOGLE_ONCE_INTERNAL_DONE, true }
  };
  // Short circuit the simplest case to avoid procedure call overhead.
  if (core::subtle::Acquire_CompareAndSwap(control, GOOGLE_ONCE_INTERNAL_INIT,
          GOOGLE_ONCE_INTERNAL_RUNNING) == GOOGLE_ONCE_INTERNAL_INIT ||
      core::internal::SpinLockWait(control, ARRAYSIZE(trans), trans) ==
      GOOGLE_ONCE_INTERNAL_INIT) {
    if (func != nullptr) {
      (*func)();
    } else {
      (*func_with_arg)(arg);
    }
    ANNOTATE_HAPPENS_BEFORE(control);
    int32 old_control = core::subtle::NoBarrier_Load(control);
    core::subtle::Release_Store(control, GOOGLE_ONCE_INTERNAL_DONE);
    if (old_control == GOOGLE_ONCE_INTERNAL_WAITER) {
      core::internal::SpinLockWake(control, true);
    }
  } // else *control is already GOOGLE_ONCE_INTERNAL_DONE
}
