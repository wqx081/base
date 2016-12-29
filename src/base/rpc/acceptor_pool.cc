#include "base/rpc/acceptor_pool.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <inttypes.h>
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "base/core/ref_counted.h"
#include "base/core/strings/substitute.h"
#include "base/rpc/messenger.h"
#include "base/util/metrics.h"
#include "base/net/sockaddr.h"
#include "base/net/socket.h"
#include "base/util/status.h"
#include "base/util/thread.h"

using google::protobuf::Message;
using std::string;

METRIC_DEFINE_counter(server, rpc_connections_accepted,
                      "RPC Connections Accepted",
                      base::MetricUnit::kConnections,
                      "Number of incoming TCP connections made to the RPC server");

DEFINE_int32(rpc_acceptor_listen_backlog, 128,
             "Socket backlog parameter used when listening for RPC connections. "
             "This defines the maximum length to which the queue of pending "
             "TCP connections inbound to the RPC server may grow. If a connection "
             "request arrives when the queue is full, the client may receive "
             "an error. Higher values may help the server ride over bursts of "
             "new inbound connection requests.");

namespace base {
namespace rpc {

AcceptorPool::AcceptorPool(Messenger* messenger, Socket* socket,
                           Sockaddr bind_address)
    : messenger_(messenger),
      socket_(socket->Release()),
      bind_address_(std::move(bind_address)),
      rpc_connections_accepted_(METRIC_rpc_connections_accepted.Instantiate(
          messenger->metric_entity())),
      closing_(false) {}

AcceptorPool::~AcceptorPool() {
  Shutdown();
}

Status AcceptorPool::Start(int num_threads) {
  RETURN_NOT_OK(socket_.Listen(FLAGS_rpc_acceptor_listen_backlog));

  for (int i = 0; i < num_threads; i++) {
    scoped_refptr<base::Thread> new_thread;
    Status s = base::Thread::Create("acceptor pool", "acceptor",
        &AcceptorPool::RunThread, this, &new_thread);
    if (!s.ok()) {
      Shutdown();
      return s;
    }
    threads_.push_back(new_thread);
  }
  return Status::OK();
}

void AcceptorPool::Shutdown() {
  if (Acquire_CompareAndSwap(&closing_, false, true) != false) {
    VLOG(2) << "Acceptor Pool on " << bind_address_.ToString()
            << " already shut down";
    return;
  }

#if defined(__linux__)
  // Closing the socket will break us out of accept() if we're in it, and
  // prevent future accepts.
  WARN_NOT_OK(socket_.Shutdown(true, true),
              strings::Substitute("Could not shut down acceptor socket on $0",
                                  bind_address_.ToString()));
#else
  // Calling shutdown on an accepting (non-connected) socket is illegal on most
  // platforms (but not Linux). Instead, the accepting threads are interrupted
  // forcefully.
  for (const scoped_refptr<base::Thread>& thread : threads_) {
    pthread_cancel(thread.get()->pthread_id());
  }
#endif

  for (const scoped_refptr<base::Thread>& thread : threads_) {
    CHECK_OK(ThreadJoiner(thread.get()).Join());
  }
  threads_.clear();
}

Sockaddr AcceptorPool::bind_address() const {
  return bind_address_;
}

Status AcceptorPool::GetBoundAddress(Sockaddr* addr) const {
  return socket_.GetSocketAddress(addr);
}

void AcceptorPool::RunThread() {
  while (true) {
    Socket new_sock;
    Sockaddr remote;
    VLOG(2) << "calling accept() on socket " << socket_.GetFd()
            << " listening on " << bind_address_.ToString();
    Status s = socket_.Accept(&new_sock, &remote, Socket::FLAG_NONBLOCKING);
    if (!s.ok()) {
      if (Release_Load(&closing_)) {
        break;
      }
      LOG(WARNING) << "AcceptorPool: accept failed: " << s.ToString();
                                 //   << THROTTLE_MSG;
      continue;
    }
    s = new_sock.SetNoDelay(true);
    if (!s.ok()) {
      LOG(WARNING) << "Acceptor with remote = " << remote.ToString()
          << " failed to set TCP_NODELAY on a newly accepted socket: "
          << s.ToString(); // << THROTTLE_MSG;
      continue;
    }
    rpc_connections_accepted_->Increment();
    messenger_->RegisterInboundSocket(&new_sock, remote);
  }
  VLOG(1) << "AcceptorPool shutting down.";
}

} // namespace rpc
} // namespace base
