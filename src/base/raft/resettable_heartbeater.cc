#include "base/raft/resettable_heartbeater.h"

#include <glog/logging.h>
#include <mutex>

#include "base/core/ref_counted.h"
#include "base/core/strings/substitute.h"
#include "base/util/countdown_latch.h"
#include "base/util/locks.h"
#include "base/util/random.h"
#include "base/util/status.h"
#include "base/util/thread.h"

namespace base {

using std::string;

class ResettableHeartbeaterThread {
 public:
  ResettableHeartbeaterThread(std::string name, MonoDelta period,
                              HeartbeatFunction function);

  Status Start();
  Status Stop();
  void Reset();

 private:
  void RunThread();
  bool IsCurrentThread() const;

  const string name_;

  const MonoDelta period_;

  const HeartbeatFunction function_;

  scoped_refptr<base::Thread> thread_;

  CountDownLatch run_latch_;

  bool shutdown_;

  mutable simple_spinlock lock_;
  DISALLOW_COPY_AND_ASSIGN(ResettableHeartbeaterThread);
};

//////////

ResettableHeartbeater::ResettableHeartbeater(const std::string& name,
                                             MonoDelta period,
                                             HeartbeatFunction function)
    : thread_(new ResettableHeartbeaterThread(name, period, function)) {
}   

Status ResettableHeartbeater::Start() {
  return thread_->Start();
} 

Status ResettableHeartbeater::Stop() {
  return thread_->Stop();
} 

void ResettableHeartbeater::Reset() {
  thread_->Reset();
} 

ResettableHeartbeater::~ResettableHeartbeater() {
  WARN_NOT_OK(Stop(), "Unable to stop heartbeater thread");
}

ResettableHeartbeaterThread::ResettableHeartbeaterThread(
    std::string name, MonoDelta period, HeartbeatFunction function)
    : name_(std::move(name)),
      period_(std::move(period)),
      function_(std::move(function)),
      run_latch_(0),
      shutdown_(false) {}

void ResettableHeartbeaterThread::RunThread() {
  CHECK(IsCurrentThread());
  VLOG(1) << "Heartbeater: " << name_ << " thread starting";

  bool prev_reset_was_manual = false;
  Random rng(random());
  while (true) {
    MonoDelta wait_period = period_;
    if (prev_reset_was_manual) {
      int64_t half_period_ms = period_.ToMilliseconds() / 2;
      wait_period = MonoDelta::FromMilliseconds(
          half_period_ms +
          rng.NextDoubleFraction() * half_period_ms);
      prev_reset_was_manual = false; 
    } 
    if (run_latch_.WaitFor(wait_period)) {
      prev_reset_was_manual = true;
      std::lock_guard<simple_spinlock> lock(lock_);
      if (shutdown_) {
        VLOG(1) << "Heartbeater: " << name_ << " thread finished";
        return; 
      } else {
        run_latch_.Reset(1);
        continue;
      } 
    } 
    
    Status s = function_();
    if (!s.ok()) {
      LOG(WARNING)<< "Failed to heartbeat in heartbeater: " << name_
      << " Status: " << s.ToString();
      continue;
    } 
  } 
} 

bool ResettableHeartbeaterThread::IsCurrentThread() const {
  return thread_.get() == base::Thread::current_thread(); 
} 

Status ResettableHeartbeaterThread::Start() {
  CHECK(thread_ == nullptr);
  run_latch_.Reset(1);
  return base::Thread::Create("heartbeater", strings::Substitute("$0-heartbeat", name_),
                              &ResettableHeartbeaterThread::RunThread,
                              this, &thread_);
}

void ResettableHeartbeaterThread::Reset() {
  if (!thread_) {
    return;
  }
  run_latch_.CountDown();
}

Status ResettableHeartbeaterThread::Stop() {
  if (!thread_) {
    return Status::OK();
  }

  {
    std::lock_guard<simple_spinlock> l(lock_);
    if (shutdown_) {
      return Status::OK();
    }
    shutdown_ = true;
  }

  run_latch_.CountDown();
  RETURN_NOT_OK(ThreadJoiner(thread_.get()).Join());
  return Status::OK();
}


} // namespace base
