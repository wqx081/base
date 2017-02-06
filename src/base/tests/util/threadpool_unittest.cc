#include <gtest/gtest.h>
#include <memory>

#include "base/core/ref_counted.h"

#include "base/util/threadpool.h"
#include "base/util/thread.h"

class FibTask : public base::Runnable {
 public:
  FibTask(int n, int number) : n_(n), number_(number) {}

  virtual void Run() override {
    int result = DoFib(n_);   
    LOG(INFO) << "Number: " << number_ << " : " << "Fib(" << n_ << "): " << result;
  }

 private:

  int DoFib(int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
      result += (i + 1);
      i++;
    }
    return result;
  }

  int n_;
  int number_;
};

void Double(int x) {
  LOG(INFO) << "Double(" << x << "): " << x * x;
}

TEST(ThreadPool, Basic) {
  base::ThreadPoolBuilder threadpool_builder("test thread pool");  

  LOG(INFO) << "max thread: " << threadpool_builder.max_threads();
//  threadpool_builder.set_max_threads(5);
  threadpool_builder.set_min_threads(2);
  threadpool_builder.set_max_queue_size(10);
  threadpool_builder.set_idle_timeout(base::MonoDelta::FromMilliseconds(100));

  gscoped_ptr<base::ThreadPool> thread_pool;
  DCHECK(threadpool_builder.Build(&thread_pool).ok()) << "Create Thread Pool Failed";

  for (int i=0; i < 5; i++) {
    std::shared_ptr<base::Runnable> task(new FibTask(i + 1, i));
    base::Status s = thread_pool->Submit(task);
    DCHECK(s.ok());
  }
  DCHECK(thread_pool->SubmitFunc(std::bind(&Double, 10)).ok());

  thread_pool->Wait();
}
