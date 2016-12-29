#include "base/rpc/constants.h"

namespace base {
namespace rpc {

const char* const kMagicNumber = "mrpc";
const char* const kSaslAppName = "mpr";
const char* const kSaslProtoName = "mpr";

std::set<RpcFeatureFlag> kSupportedServerRpcFeatureFlags = { APPLICATION_FEATURE_FLAGS };
std::set<RpcFeatureFlag> kSupportedClientRpcFeatureFlags = { APPLICATION_FEATURE_FLAGS };

} // namespace rpc
} // namespace base
