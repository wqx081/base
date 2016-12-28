#ifndef BASE_UTIL_THREAD_H_
#define BASE_UTIL_THREAD_H_

// 当前只支持 pthread
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <string>
#include <vector>

#include <functional>

#include "base/core/atomicops.h"
#include "base/core/ref_counted.h"
#include "base/util/synchronizer.h"
#include "base/util/status.h"

namespace base {

class Thread;
class WebCallbackRegistry;

// @class ThreadJoiner 主要用于join 一个线程,
// 而且根据join期间的时长打印消息.

class ThreadJoiner {
 public:
  explicit ThreadJoiner(Thread* thread);
  ThreadJoiner& warn_after_ms(int ms);
  ThreadJoiner& warn_every_ms(int ms);
  ThreadJoiner& give_up_after_ms(int ms);

  Status Join();
 private:
  enum {
    kDefaultWarnAfterMs = 1000,
    kDefaultWarnEveryMs = 1000,
    kDefaultGiveUpAfterMs = -1
  };

  Thread* thread_;
 
  int warn_after_ms_;
  int warn_every_ms_;
  int give_up_after_ms_;

  DISALLOW_COPY_AND_ASSIGN(ThreadJoiner);
};

// @class Thread 对pthread 进行封装, 并且把自身注册到单例 ThreadManager, 用于监控跟踪所有存活的 Thread.
// 每个 Thread 必须提供 category 和 name, 我们在Web UI 监控界面中可以显示该 Thread.
//
//
class Thread : public core::RefCountedThreadSafe<Thread> {
 public:
  enum CreateFlags {
    NO_FLAGS = 0,
    NO_STACK_WATCHDOG = 1 << 0
  };
  
  template<typename F>
  static Status CreateWithFlags(const std::string& category,
                                const std::string& name,
                                const F& f,
                                uint64_t flags,
                                scoped_refptr<Thread>* holder) {
    return StartThread(category, name, f, flags, holder);

  }
  template <class F>
  static Status Create(const std::string& category, const std::string& name, const F& f,
                       scoped_refptr<Thread>* holder) {
    return StartThread(category, name, f, NO_FLAGS, holder);
  }

  template <class F, class A1>
  static Status Create(const std::string& category, const std::string& name, const F& f,
                       const A1& a1, scoped_refptr<Thread>* holder) {
    return StartThread(category, name, std::bind(f, a1), NO_FLAGS, holder);
  }

  template <class F, class A1, class A2>
  static Status Create(const std::string& category, const std::string& name, const F& f,
                       const A1& a1, const A2& a2, scoped_refptr<Thread>* holder) {
    return StartThread(category, name, std::bind(f, a1, a2), NO_FLAGS, holder);
  }

  template <class F, class A1, class A2, class A3>
  static Status Create(const std::string& category, const std::string& name, const F& f,
                       const A1& a1, const A2& a2, const A3& a3, scoped_refptr<Thread>* holder) {
    return StartThread(category, name, std::bind(f, a1, a2, a3), NO_FLAGS, holder);
  }

  template <class F, class A1, class A2, class A3, class A4>
  static Status Create(const std::string& category, const std::string& name, const F& f,
                       const A1& a1, const A2& a2, const A3& a3, const A4& a4,
                       scoped_refptr<Thread>* holder) {
    return StartThread(category, name, std::bind(f, a1, a2, a3, a4), NO_FLAGS, holder);
  }

  template <class F, class A1, class A2, class A3, class A4, class A5>
  static Status Create(const std::string& category, const std::string& name, const F& f,
                       const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5,
                       scoped_refptr<Thread>* holder) {
    return StartThread(category, name, std::bind(f, a1, a2, a3, a4, a5), NO_FLAGS, holder);
  }

  template <class F, class A1, class A2, class A3, class A4, class A5, class A6>
  static Status Create(const std::string& category, const std::string& name, const F& f,
                       const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5,
                       const A6& a6, scoped_refptr<Thread>* holder) {
    return StartThread(category, name, std::bind(f, a1, a2, a3, a4, a5, a6), NO_FLAGS, holder);
  }


  
  ~Thread();

  void Join() { ThreadJoiner(this).Join(); }

  void CallAtExit(const core::Closure& cb);

  int64_t tid() const { return tid_; }

  pthread_t pthread_id() const { return thread_; }

  const std::string& name() const { return name_; }

  const std::string& category() const { return category_; }

  std::string ToString() const;

  static Thread* current_thread() { return tls_; }

  static int64_t UniqueThreadId() {
    return static_cast<int64_t>(pthread_self());
  }

  static int64_t CurrentThreadId() {
    return syscall(SYS_gettid);
  }

 private:
  friend class ThreadJoiner;

  enum {
    INVALID_TID = -1,
    CHILD_WAITING_TID = -2,
    PARENT_WAITING_TID = -3,
  };

  typedef std::function<void()> ThreadFunctor;

  Thread(std::string category, std::string name, ThreadFunctor functor)
    : thread_(0),
      category_(std::move(category)),
      name_(std::move(name)),
      tid_(CHILD_WAITING_TID),
      functor_(std::move(functor)),
      done_(1),
      joinable_(false) {}

  pthread_t thread_;
  
  const std::string category_;
  const std::string name_;

  int64_t tid_;
  const ThreadFunctor functor_;

  CountDownLatch done_;

  bool joinable_;

  // Thread local pointer to the current thread of execution.
  // Will be NULL if the current thread is not a thread.
  static __thread Thread* tls_;

  std::vector<core::Closure> exit_callbacks_;

  static Status StartThread(const std::string& category,
                            const std::string& name,
                            const ThreadFunctor& functor,
                            uint64_t flags,
                            scoped_refptr<Thread>* holder);

  static void* SuperviseThread(void* arg);
  static void FinishThread(void* arg);
};


//TODO(wqx):
//Status StartThreadInstrumentation(const scoped_refptr<MetricEntity>& server_metrics,
//                                  WebCallbackRegistry* web);

} // namespace base
#endif // BASE_UTIL_THREAD_H_
