#ifndef BASE_RPC_CONSTANTS_H_
#define BASE_RPC_CONSTANTS_H_
#include <cstdint>
#include <set>

#include "base/rpc/rpc_header.pb.h"

namespace base {
namespace rpc {

extern const char* const kMagicNumber;
extern const char* const kSaslAppName;
extern const char* const kSaslProtoName;

static const uint32_t kCurrentRpcVersion = 9;

static const int32_t kInvalidCallId = -2;
static const int32_t kConnectionContextCallId = -3;
static const int32_t kSaslCallId = -33;

static const uint8_t kMagicNumberLength = 4;
static const uint8_t kHeaderFlagsLength = 3;

static const uint8_t kMsgLengthPrefixLength = 4;

extern std::set<RpcFeatureFlag> kSupportedServerRpcFeatureFlags;
extern std::set<RpcFeatureFlag> kSupportedClientRpcFeatureFlags;

} // namespace rpc
} // namespace base
#endif // BASE_RPC_CONSTANTS_H_
