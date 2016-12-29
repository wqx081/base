#ifndef BASE_NET_SSL_FACTORY_H_
#define BASE_NET_SSL_FACTORY_H_

#include <string>
#include <memory>

#include "base/core/macros.h"
#include "base/core/strings/substitute.h"
#include "base/util/errno.h"
#include "base/util/status.h"

struct ssl_ctx_st;
typedef ssl_ctx_st SSL_CTX;

namespace base {

class Sockaddr;
class SSLSocket;

class SSLFactory {
 public:
  SSLFactory();

  ~SSLFactory();

  // Set up the SSL_CTX and choose encryption preferences.
  Status Init();

  // Load the server certificate.
  Status LoadCertificate(const std::string& certificate_path);

  // Load the private key for the server certificate.
  Status LoadPrivateKey(const std::string& key_path);

  // Load the certificate authority.
  Status LoadCertificateAuthority(const std::string& certificate_path);

  // Create an SSLSocket wrapped around the file descriptor 'socket_fd'. 'is_server' denotes if it's
  // a server socket. The 'socket_fd' is closed when this object is destroyed.
  std::unique_ptr<SSLSocket> CreateSocket(int socket_fd, bool is_server);

 private:
  friend class SSLSocket;
  std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>> ctx_;

  // Gets the last error from the thread local SSL error queue. If no error exists, it returns
  // the error corresponding to 'errno_copy'.
  static std::string GetLastError(int errno_copy);
};

} // namespace base

#endif // BASE_NET_SSL_FACTORY_H_
