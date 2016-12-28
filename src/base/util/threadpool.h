#ifndef BASE_UTIL_THREAD_POOL_H_
#define BASE_UTIL_THREAD_POOL_H_

#include <functional>
#include <list>
#include <memory>
#include <unordered_set>
#include <string>
#include <vector>

#include "base/core/callback_forward.h"
#include "base/core/gscoped_ptr.h"
#include "base/core/macros.h"
#include "base/core/port.h"
#include "base/core/ref_counted.h"

#include "base/util/condition_variable.h"
#include "base/util/monotime.h"
#include "base/util/mutex.h"
#include "base/util/status.h"

namespace base {

class Thread;
class ThreadPool;

class Runnable {
 public:
  virtual void Run() = 0;
  virtual ~Runnable() {}
};

class ThreadPoolBuilder {
 public:
  explicit ThreadPoolBuilder(std::string name);

  ThreadPoolBuilder& set_min_threads(int min_threads);
  ThreadPoolBuilder& set_max_threads(int max_threads);
  ThreadPoolBuilder& set_max_queue_size(int max_queue_size);
  ThreadPoolBuilder& set_idle_timeout(const MonoDelta& idle_timeout);

  const std::string& name() const { return name_; }
  int min_threads() const { return min_threads_; }
  int max_threads() const { return max_threads_; }
  int max_queue_size() const { return max_queue_size_; }
  const MonoDelta& idle_timeout() const { return idle_timeout_; }

  Status Build(gscoped_ptr<ThreadPool>* pool) const;

 private:
  friend class ThreadPool;
  const std::string name_;
  int min_threads_;
  int max_threads_;
  int max_queue_size_;
  MonoDelta idle_timeout_;

  DISALLOW_COPY_AND_ASSIGN(ThreadPoolBuilder);
};

class ThreadPool {
 public:
  ~ThreadPool();

  void Shutdown();

  Status SubmitClosure(const core::Closure& task);
  Status SubmitFunc(const std::function<void()>& func);
  Status Submit(const std::shared_ptr<Runnable>& task);
  
  void Wait();
  bool WaitUntil(const MonoTime& until);
  bool WaitFor(const MonoDelta& delta);

  int queue_length() const;
  
 private:
  friend class ThreadPoolBuilder;

  explicit ThreadPool(const ThreadPoolBuilder& builder);
  
  Status Init();
  void DispatchThread(bool permanent);

  Status CreateThreadUnlocked();
  void CheckNotPoolThreadUnlocked();

 private:
  struct QueueEntry {
    std::shared_ptr<Runnable> runnable;

    // TODO(wqx):
    // Trace* trace;

    MonoTime submit_time;
  };

  const std::string name_;
  const int min_threads_;
  const int max_threads_;
  const int max_queue_size_;
  const MonoDelta idle_timeout_;

  Status pool_status_;
  Mutex lock_;
  ConditionVariable idle_cond_;
  ConditionVariable no_threads_cond_;
  ConditionVariable not_empty_;
  int num_threads_;
  int active_threads_;
  int queue_size_;
  std::list<QueueEntry> queue_;

  std::unordered_set<Thread*> threads_;

  //TODO(wqx):
  // metrics

  DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};

} // namespace base
#endif // BASE_UTIL_THREAD_POOL_H_
