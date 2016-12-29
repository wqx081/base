#include "base/util/pb_util.h"

#include <deque>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include <glog/logging.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor_database.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/message.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/text_format.h>

#include "base/core/bind.h"
#include "base/core/callback.h"
#include "base/core/map_util.h"
#include "base/core/strings/escaping.h"
#include "base/core/strings/fastmem.h"
#include "base/core/strings/substitute.h"
#include "base/util/env.h"
#include "base/util/env_util.h"
#include "base/util/jsonwriter.h"
#include "base/util/mutex.h"
#include "base/util/path_util.h"
#include "base/util/status.h"

using google::protobuf::Descriptor;
using google::protobuf::DescriptorPool;
using google::protobuf::DynamicMessageFactory;
using google::protobuf::FieldDescriptor;
using google::protobuf::FileDescriptor;
using google::protobuf::FileDescriptorProto;
using google::protobuf::FileDescriptorSet;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::Message;
using google::protobuf::MessageLite;
using google::protobuf::Reflection;
using google::protobuf::SimpleDescriptorDatabase;
using google::protobuf::TextFormat;
using std::deque;
using std::endl;
using std::initializer_list;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::unordered_set;
using std::vector;
using strings::Substitute;
using strings::Utf8SafeCEscape;

namespace std {

// Allow the use of FileState with DCHECK_EQ.
std::ostream& operator<< (std::ostream& os, const base::pb_util::FileState& state) {
  os << static_cast<int>(state);
  return os;
}

} // namespace std

namespace base {
namespace pb_util {

static const char* const kTmpTemplateSuffix = ".XXXXXX";

// Protobuf container constants.
static const uint32_t kPBContainerInvalidVersion = 0;
static const uint32_t kPBContainerDefaultVersion = 2;
static const int kPBContainerChecksumLen = sizeof(uint32_t);
static const char kPBContainerMagic[] = "kuducntr";
static const int kPBContainerMagicLen = 8;
static const int kPBContainerV1HeaderLen =
    kPBContainerMagicLen + sizeof(uint32_t); // Magic number + version.
static const int kPBContainerV2HeaderLen =
    kPBContainerV1HeaderLen + kPBContainerChecksumLen; // Same as V1 plus a checksum.

const int kPBContainerMinimumValidLength = kPBContainerV1HeaderLen;

static_assert(arraysize(kPBContainerMagic) - 1 == kPBContainerMagicLen,
              "kPBContainerMagic does not match expected length");


namespace {
class SecureFieldPrinter : public TextFormat::FieldValuePrinter {
 public:
  using super = TextFormat::FieldValuePrinter;

  string PrintFieldName(const Message& message,
                                                const Reflection* reflection,
                                                const FieldDescriptor* field) const override {
    hide_next_string_ = field->cpp_type() == FieldDescriptor::CPPTYPE_STRING &&
        field->options().GetExtension(REDACT);
    return super::PrintFieldName(message, reflection, field);
  }

  string PrintString(const string& val) const override {
    if (hide_next_string_) {
      hide_next_string_ = false;
      return KUDU_REDACT(super::PrintString(val));
    }
    return super::PrintString(val);
  }
  string PrintBytes(const string& val) const override {
    if (hide_next_string_) {
      hide_next_string_ = false;
      return KUDU_REDACT(super::PrintString(val));
    }
    return super::PrintBytes(val);
  }

  mutable bool hide_next_string_ = false;
};
} // anonymous namespace

string SecureDebugString(const Message& msg) {
  string debug_string;
  TextFormat::Printer printer;
  printer.SetDefaultFieldValuePrinter(new SecureFieldPrinter());
  printer.PrintToString(msg, &debug_string);
  return debug_string;
}

string SecureShortDebugString(const Message& msg) {
  string debug_string;

  TextFormat::Printer printer;
  printer.SetSingleLineMode(true);
  printer.SetDefaultFieldValuePrinter(new SecureFieldPrinter());

  printer.PrintToString(msg, &debug_string);
  if (!debug_string.empty() &&
            debug_string[debug_string.size() - 1] == ' ') {
    debug_string.resize(debug_string.size() - 1);
  }

  return debug_string;
}


} // namespace pb_util
} // namespace base
