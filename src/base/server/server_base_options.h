#ifndef BASE_SERVER_SERVER_BASE_OPTIONS_H_
#define BASE_SERVER_SERVER_BASE_OPTIONS_H_
#include <string>
#include <vector>

#include "base/server/rpc_server.h"

namespace base {

class Env;

namespace server {

struct ServerBaseOptions {
  Env* env;

  RpcServerOptions rpc_opts;

 protected:
  ServerBaseOptions();
};

} // namespace server
} // namespace base
#endif // BASE_SERVER_SERVER_BASE_OPTIONS_H_
