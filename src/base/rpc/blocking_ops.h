#ifndef BASE_RPC_BLOCKING_OPS_H
#define BASE_RPC_BLOCKING_OPS_H

#include <set>
#include <string>

namespace google {
namespace protobuf {
class MessageLite;
} // namespace protobuf
} // namespace google

namespace base {

class faststring;
class MonoTime;
class Slice;
class Sockaddr;
class Socket;
class Status;

namespace rpc {

class SaslMessagePB;

// Returns OK if socket is in blocking mode. Otherwise, returns an error.
Status EnsureBlockingMode(const Socket* const sock);

// Encode and send a message over a socket.
// header: Request or Response header protobuf.
// msg: Protobuf message to send. This message must be fully initialized.
// deadline: Latest time allowed for receive to complete before timeout.
Status SendFramedMessageBlocking(Socket* sock, const google::protobuf::MessageLite& header,
    const google::protobuf::MessageLite& msg, const MonoTime& deadline);

// Receive a full message frame from the server.
// recv_buf: buffer to use for reading the data from the socket.
// header: Request or Response header protobuf.
// param_buf: Slice into recv_buf containing unparsed RPC param protobuf data.
// deadline: Latest time allowed for receive to complete before timeout.
Status ReceiveFramedMessageBlocking(Socket* sock, faststring* recv_buf,
    google::protobuf::MessageLite* header, Slice* param_buf, const MonoTime& deadline);

} // namespace rpc
} // namespace base

#endif  // BASE_RPC_BLOCKING_OPS_H
