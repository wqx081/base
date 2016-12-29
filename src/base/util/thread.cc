#include "base/util/thread.h"

#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

#include <sys/prctl.h>

#include "base/core/atomicops.h"
#include "base/core/dynamic_annotations.h"
#include "base/core/mathlimits.h"
#include "base/core/once.h"
#include "base/core/strings/substitute.h"

#include "base/util/errno.h"
#include "base/util/mutex.h"
#include "base/util/stopwatch.h"
#include "base/util/yield.h"
#include "base/http/web_callback_registry.h"

namespace base {

class ThreadManager;

__thread Thread* Thread::tls_ = nullptr;

static std::shared_ptr<ThreadManager> thread_manager;
static GoogleOnceType once = GOOGLE_ONCE_INIT;

// ThreadManager
class ThreadManager {
 public:
  ThreadManager() {}

  ~ThreadManager() {
    MutexLock l(lock_);
    thread_categories_.clear();
  }

  static void SetThreadName(const std::string& name, int64 tid);

  void AddThread(const pthread_t& pthread_id, 
                 const std::string& name, 
                 const std::string& category,
                 int64_t tid);

  void RemoveThread(const pthread_t& pthread_id,
                    const std::string& category);

 private:
  class ThreadDescriptor {
   public:
    ThreadDescriptor() {}
    ThreadDescriptor(std::string category, std::string name, int64_t tid)
      : name_(std::move(name)),
        category_(std::move(category)),
        thread_id_(tid) {}

    const std::string& name() const { return name_; }
    const std::string& category() const { return category_; }
    int64_t thread_id() const { return thread_id_; }

   private:
    std::string name_;
    std::string category_;
    int64_t thread_id_;
  };

  typedef std::map<const pthread_t, ThreadDescriptor> ThreadCategory;
  typedef std::map<std::string, ThreadCategory> ThreadCategoryMap;

  Mutex lock_;
  ThreadCategoryMap thread_categories_;
  
  // TODO(wqx):
  // supported metrics
};

void ThreadManager::SetThreadName(const std::string& name, int64 tid) {
  if (tid == getpid()) {
    return;
  }

  int err = prctl(PR_SET_NAME, name.c_str());
  if (err < 0 && errno != EPERM) {
    PLOG(ERROR) << "SetThreadName";
  }
}

void ThreadManager::AddThread(const pthread_t& pthread_id,
                              const std::string& name,
                              const std::string& category,
                              int64_t tid) {
  ANNOTATE_IGNORE_SYNC_BEGIN();
  ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN();
  {
    MutexLock l(lock_);
    thread_categories_[category][pthread_id] = ThreadDescriptor(category, name, tid);
  }
  ANNOTATE_IGNORE_SYNC_END();
  ANNOTATE_IGNORE_READS_AND_WRITES_END();
}

//
static void InitThreading() {
  //TODO(wqx):
  thread_manager.reset(new ThreadManager());
}

// ThreadJoiner
//
ThreadJoiner::ThreadJoiner(Thread* thread)
  : thread_(CHECK_NOTNULL(thread)), 
    warn_after_ms_(kDefaultWarnAfterMs),
    warn_every_ms_(kDefaultWarnEveryMs),
    give_up_after_ms_(kDefaultGiveUpAfterMs) {
}

ThreadJoiner& ThreadJoiner::warn_after_ms(int ms) {
  warn_after_ms_ = ms;
  return *this;
}

ThreadJoiner& ThreadJoiner::warn_every_ms(int ms) {
  warn_every_ms_ = ms;
  return *this;
}

ThreadJoiner& ThreadJoiner::give_up_after_ms(int ms) {
  give_up_after_ms_ = ms;
  return *this;
}

Status ThreadJoiner::Join() {
  if (Thread::current_thread() &&
            Thread::current_thread()->tid() == thread_->tid()) {
    return Status::InvalidArgument("Can't join on own thread", thread_->name_);
  }

  if (!thread_->joinable_) {
    return Status::OK();
  }

  int waited_ms = 0;
  bool keep_trying = true;
  while (keep_trying) {
    if (waited_ms >= warn_after_ms_) {
      LOG(WARNING) << strings::Substitute("Waited for $0ms trying to join with $1 (tid $2)",
                                 waited_ms, thread_->name_, thread_->tid_);
    }

    int remaining_before_giveup = MathLimits<int>::kMax;
    if (give_up_after_ms_ != -1) {
      remaining_before_giveup = give_up_after_ms_ - waited_ms;
    }

    int remaining_before_next_warn = warn_every_ms_;
    if (waited_ms < warn_after_ms_) {
      remaining_before_next_warn = warn_after_ms_ - waited_ms;
    }

    if (remaining_before_giveup < remaining_before_next_warn) {
      keep_trying = false;
    }

    int wait_for = std::min(remaining_before_giveup, remaining_before_next_warn);

    if (thread_->done_.WaitFor(MonoDelta::FromMilliseconds(wait_for))) {
      int ret = pthread_join(thread_->thread_, NULL);
      CHECK_EQ(ret, 0);
      thread_->joinable_ = false;
      return Status::OK();
    }
    waited_ms += wait_for;
  }
  return Status::Aborted(strings::Substitute("Timed out after $0ms joining on $1",
                                             waited_ms, thread_->name_));
}

// Thread
Thread::~Thread() {
  if (joinable_) {
    int ret = pthread_detach(thread_);
    CHECK_EQ(ret, 0);
  }
}

void Thread::CallAtExit(const core::Closure& cb) {
  CHECK_EQ(Thread::current_thread(), this);
  exit_callbacks_.push_back(cb);
}

std::string Thread::ToString() const {
  return strings::Substitute("Thread $0 (name: \"$1\", category: \"$2\")", tid_, name_, category_);
}


Status Thread::StartThread(const std::string& category, const std::string& name,
                           const ThreadFunctor& functor, uint64_t flags,
                           scoped_refptr<Thread> *holder) { 

  scoped_refptr<Thread> t(new Thread(category, name, functor));
  { 
    int ret = pthread_create(&t->thread_, NULL, &Thread::SuperviseThread, t.get());
    if (ret) { 
      return Status::RuntimeError("Could not create thread", strerror(ret), ret);
    } 
  } 

  t->joinable_ = true;
  if (holder) { 
    *holder = t;
  } 
  Release_Store(&t->tid_, PARENT_WAITING_TID);
  { 
    int loop_count = 0;
    while (Acquire_Load(&t->tid_) == PARENT_WAITING_TID) { 
      yield(loop_count++);
    } 
  } 

  VLOG(2) << "Started thread " << t->tid()<< " - " << category << ":" << name;
  return Status::OK();
}

void* Thread::SuperviseThread(void* arg) {
  Thread* t = static_cast<Thread*>(arg);
  int64_t system_tid = Thread::CurrentThreadId();
  if (system_tid == -1) {
    string error_msg = ErrnoToString(errno);
    LOG(ERROR) << "Could not determine thread ID: " << error_msg;
  }
  string name = strings::Substitute("$0-$1", t->name(), system_tid);
  
  GoogleOnceInit(&once, &InitThreading);
  ANNOTATE_IGNORE_SYNC_BEGIN();
  std::shared_ptr<ThreadManager> thread_mgr_ref = thread_manager;
  ANNOTATE_IGNORE_SYNC_END();
  
  scoped_refptr<Thread> thread_ref = t;
  t->tls_ = t;
  
  int loop_count = 0;
  while (Acquire_Load(&t->tid_) == CHILD_WAITING_TID) {
    yield(loop_count++);
  }
  Release_Store(&t->tid_, system_tid);
  
  thread_manager->SetThreadName(name, t->tid());
  thread_manager->AddThread(pthread_self(), name, t->category(), t->tid());
  
  pthread_cleanup_push(&Thread::FinishThread, t);
  t->functor_();
  pthread_cleanup_pop(true);
  
  return NULL;
} 

void Thread::FinishThread(void* arg) {
  Thread* t = static_cast<Thread*>(arg);
  
  for (core::Closure& c : t->exit_callbacks_) {
    c.Run();
  } 
  
  thread_manager->RemoveThread(pthread_self(), t->category());

  t->done_.CountDown();
  
  VLOG(2) << "Ended thread " << t->tid() << " - "
          << t->category() << ":" << t->name();
}         

} // namespace base
