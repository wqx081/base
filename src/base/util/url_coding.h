#ifndef UTIL_URL_CODING_H
#define UTIL_URL_CODING_H

#include <stdint.h>

#include <iosfwd>
#include <string>
#include <vector>

namespace base {

// Utility method to URL-encode a string (that is, replace special
// characters with %<hex value in ascii>).
// The optional parameter hive_compat controls whether we mimic Hive's
// behaviour when encoding a string, which is only to encode certain
// characters (excluding, e.g., ' ')
void UrlEncode(const std::string& in, std::string* out, bool hive_compat = false);
void UrlEncode(const std::vector<uint8_t>& in, std::string* out,
    bool hive_compat = false);
std::string UrlEncodeToString(const std::string& in, bool hive_compat = false);

// Utility method to decode a string that was URL-encoded. Returns
// true unless the string could not be correctly decoded.
// The optional parameter hive_compat controls whether or not we treat
// the strings as encoded by Hive, which means selectively ignoring
// certain characters like ' '.
bool UrlDecode(const std::string& in, std::string* out, bool hive_compat = false);

// Utility method to encode input as base-64 encoded.  This is not
// very performant (multiple string copies) and should not be used
// in a hot path.
void Base64Encode(const std::vector<uint8_t>& in, std::string* out);
void Base64Encode(const std::vector<uint8_t>& in, std::ostringstream* out);
void Base64Encode(const std::string& in, std::string* out);
void Base64Encode(const std::string& in, std::ostringstream* out);

// Utility method to decode base64 encoded strings.  Also not extremely
// performant.
// Returns true unless the string could not be correctly decoded.
bool Base64Decode(const std::string& in, std::string* out);

// Replaces &, < and > with &amp;, &lt; and &gt; respectively. This is
// not the full set of required encodings, but one that should be
// added to on a case-by-case basis. Slow, since it necessarily
// inspects each character in turn, and copies them all to *out; use
// judiciously.
void EscapeForHtml(const std::string& in, std::ostringstream* out);

// Same as above, but returns a string.
std::string EscapeForHtmlToString(const std::string& in);

} // namespace base
#endif
