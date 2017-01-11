#ifndef KUDU_UTIL_HEXDUMP_H
#define KUDU_UTIL_HEXDUMP_H

#include <string>

namespace base {

class Slice;

// Generate an 'xxd'-style hexdump of the given slice.  This should only be used
// for debugging, as the format is subject to change and it has not been
// implemented for speed.
//
// The returned string will be redacted if redaction is enabled.
std::string HexDump(const Slice &slice);

} // namespace base
#endif
