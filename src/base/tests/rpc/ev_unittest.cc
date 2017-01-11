#include <ev++.h>
#include <boost/intrusive/list.hpp>

#include <functional>
#include <memory>

#include <glog/logging.h>

#include "base/core/ref_counted.h"
#include "base/util/thread.h"
#include "base/util/locks.h"
#include "base/util/status.h"

#include "base/net/socket.h"

#include "base/rpc/messenger.h"
#include "base/rpc/connection.h"

namespace base {
namespace test {

class Reactor;
class ReactorThread;

class ReactorTask : public boost::intrusive::list_base_hook<> {
 public:
  ReactorTask();
  virtual ~ReactorTask();

  virtual void Run(ReactorThread* reactor) = 0;
  virtual void Abort(const Status& abort_status) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ReactorTask);
};

class Connection {
 public:
  Connection();
  ~Connection() {}
 private:
};

class ReactorThread {
 public:

  ReactorThread(Reactor* reactor, const rpc::MessengerBuilder& builder)
    : loop_(ev::EPOLL),
      cur_time_(MonoTime::Now()),
      last_unused_tcp_scan_(cur_time_),
      reactor_(reactor) {}

  Status Start() {
    async_.set(loop_);
    async_.set<ReactorThread, &ReactorThread::AsyncHandler>(this);
    async_.start();

    return base::Thread::Create("reactor", "rpc reactor", &ReactorThread::RunThread, this, &thread_);
  }

  void Shutdown();
  void WakeThread();

  void AsyncHandler(ev::async& watcher, int revents);
  void TimerHandler(ev::timer& watcher, int revents);

  void RegisterTimeout(ev::timer* watcher);
  const std::string& name() const;

  MonoTime cur_time() const;
  Reactor* reactor();

  bool IsCurrentThread() const;
  
 private:
  void RunThread();

  scoped_refptr<base::Thread> thread_;
  ev::dynamic_loop loop_;
  ev::async async_;
  ev::timer timer_;

  MonoTime cur_time_;
  MonoTime last_unused_tcp_scan_;

  //TODO:
  // Handle Connect

  Reactor* reactor_;

  const MonoDelta connection_keepalive_time_;
  const MonoDelta coarse_timer_granularity_;
};

class Reactor {
 public:
  Reactor();
  Status Start();

 private:
};

} // namespace test
} // namespace base
