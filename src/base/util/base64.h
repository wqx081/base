#ifndef BASE_UTIL_BASE64_H_
#define BASE_UTIL_BASE64_H_

#include <string>

#include "base/util/status.h"
#include "base/core/strings/stringpiece.h"

namespace base {

Status Base64Encode(StringPiece data, bool with_padding, std::string* encoded);
Status Base64Encode(StringPiece data, std::string* encoded);  // with_padding=false.

Status Base64Decode(StringPiece data, std::string* decoded);

} // namespace base 
#endif // MCS_BASE_STRINGS_BASE64_H_
