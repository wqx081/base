// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: base/raft/proto/log.proto

#ifndef PROTOBUF_base_2fraft_2fproto_2flog_2eproto__INCLUDED
#define PROTOBUF_base_2fraft_2fproto_2flog_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "base/common/common.pb.h"
#include "base/raft/proto/consensus.pb.h"
#include "base/raft/proto/metadata.pb.h"
// @@protoc_insertion_point(includes)

namespace base {
namespace log {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_base_2fraft_2fproto_2flog_2eproto();
void protobuf_AssignDesc_base_2fraft_2fproto_2flog_2eproto();
void protobuf_ShutdownFile_base_2fraft_2fproto_2flog_2eproto();

class LogEntryPB;
class LogEntryBatchPB;
class LogSegmentHeaderPB;
class LogSegmentFooterPB;

enum LogEntryTypePB {
  UNKNOWN = 0,
  REPLICATE = 1,
  COMMIT = 2,
  FLUSH_MARKER = 999
};
bool LogEntryTypePB_IsValid(int value);
const LogEntryTypePB LogEntryTypePB_MIN = UNKNOWN;
const LogEntryTypePB LogEntryTypePB_MAX = FLUSH_MARKER;
const int LogEntryTypePB_ARRAYSIZE = LogEntryTypePB_MAX + 1;

const ::google::protobuf::EnumDescriptor* LogEntryTypePB_descriptor();
inline const ::std::string& LogEntryTypePB_Name(LogEntryTypePB value) {
  return ::google::protobuf::internal::NameOfEnum(
    LogEntryTypePB_descriptor(), value);
}
inline bool LogEntryTypePB_Parse(
    const ::std::string& name, LogEntryTypePB* value) {
  return ::google::protobuf::internal::ParseNamedEnum<LogEntryTypePB>(
    LogEntryTypePB_descriptor(), name, value);
}
// ===================================================================

class LogEntryPB : public ::google::protobuf::Message {
 public:
  LogEntryPB();
  virtual ~LogEntryPB();

  LogEntryPB(const LogEntryPB& from);

  inline LogEntryPB& operator=(const LogEntryPB& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LogEntryPB& default_instance();

  void Swap(LogEntryPB* other);

  // implements Message ----------------------------------------------

  LogEntryPB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LogEntryPB& from);
  void MergeFrom(const LogEntryPB& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required .base.log.LogEntryTypePB type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::base::log::LogEntryTypePB type() const;
  inline void set_type(::base::log::LogEntryTypePB value);

  // optional .base.consensus.ReplicateMsg replicate = 2;
  inline bool has_replicate() const;
  inline void clear_replicate();
  static const int kReplicateFieldNumber = 2;
  inline const ::base::consensus::ReplicateMsg& replicate() const;
  inline ::base::consensus::ReplicateMsg* mutable_replicate();
  inline ::base::consensus::ReplicateMsg* release_replicate();
  inline void set_allocated_replicate(::base::consensus::ReplicateMsg* replicate);

  // optional .base.consensus.CommitMsg commit = 3;
  inline bool has_commit() const;
  inline void clear_commit();
  static const int kCommitFieldNumber = 3;
  inline const ::base::consensus::CommitMsg& commit() const;
  inline ::base::consensus::CommitMsg* mutable_commit();
  inline ::base::consensus::CommitMsg* release_commit();
  inline void set_allocated_commit(::base::consensus::CommitMsg* commit);

  // @@protoc_insertion_point(class_scope:base.log.LogEntryPB)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_replicate();
  inline void clear_has_replicate();
  inline void set_has_commit();
  inline void clear_has_commit();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::base::consensus::ReplicateMsg* replicate_;
  ::base::consensus::CommitMsg* commit_;
  int type_;
  friend void  protobuf_AddDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_AssignDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_ShutdownFile_base_2fraft_2fproto_2flog_2eproto();

  void InitAsDefaultInstance();
  static LogEntryPB* default_instance_;
};
// -------------------------------------------------------------------

class LogEntryBatchPB : public ::google::protobuf::Message {
 public:
  LogEntryBatchPB();
  virtual ~LogEntryBatchPB();

  LogEntryBatchPB(const LogEntryBatchPB& from);

  inline LogEntryBatchPB& operator=(const LogEntryBatchPB& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LogEntryBatchPB& default_instance();

  void Swap(LogEntryBatchPB* other);

  // implements Message ----------------------------------------------

  LogEntryBatchPB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LogEntryBatchPB& from);
  void MergeFrom(const LogEntryBatchPB& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .base.log.LogEntryPB entry = 1;
  inline int entry_size() const;
  inline void clear_entry();
  static const int kEntryFieldNumber = 1;
  inline const ::base::log::LogEntryPB& entry(int index) const;
  inline ::base::log::LogEntryPB* mutable_entry(int index);
  inline ::base::log::LogEntryPB* add_entry();
  inline const ::google::protobuf::RepeatedPtrField< ::base::log::LogEntryPB >&
      entry() const;
  inline ::google::protobuf::RepeatedPtrField< ::base::log::LogEntryPB >*
      mutable_entry();

  // @@protoc_insertion_point(class_scope:base.log.LogEntryBatchPB)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::base::log::LogEntryPB > entry_;
  friend void  protobuf_AddDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_AssignDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_ShutdownFile_base_2fraft_2fproto_2flog_2eproto();

  void InitAsDefaultInstance();
  static LogEntryBatchPB* default_instance_;
};
// -------------------------------------------------------------------

class LogSegmentHeaderPB : public ::google::protobuf::Message {
 public:
  LogSegmentHeaderPB();
  virtual ~LogSegmentHeaderPB();

  LogSegmentHeaderPB(const LogSegmentHeaderPB& from);

  inline LogSegmentHeaderPB& operator=(const LogSegmentHeaderPB& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LogSegmentHeaderPB& default_instance();

  void Swap(LogSegmentHeaderPB* other);

  // implements Message ----------------------------------------------

  LogSegmentHeaderPB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LogSegmentHeaderPB& from);
  void MergeFrom(const LogSegmentHeaderPB& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 major_version = 1;
  inline bool has_major_version() const;
  inline void clear_major_version();
  static const int kMajorVersionFieldNumber = 1;
  inline ::google::protobuf::uint32 major_version() const;
  inline void set_major_version(::google::protobuf::uint32 value);

  // required uint32 minor_version = 2;
  inline bool has_minor_version() const;
  inline void clear_minor_version();
  static const int kMinorVersionFieldNumber = 2;
  inline ::google::protobuf::uint32 minor_version() const;
  inline void set_minor_version(::google::protobuf::uint32 value);

  // required bytes tablet_id = 5;
  inline bool has_tablet_id() const;
  inline void clear_tablet_id();
  static const int kTabletIdFieldNumber = 5;
  inline const ::std::string& tablet_id() const;
  inline void set_tablet_id(const ::std::string& value);
  inline void set_tablet_id(const char* value);
  inline void set_tablet_id(const void* value, size_t size);
  inline ::std::string* mutable_tablet_id();
  inline ::std::string* release_tablet_id();
  inline void set_allocated_tablet_id(::std::string* tablet_id);

  // required uint64 sequence_number = 6;
  inline bool has_sequence_number() const;
  inline void clear_sequence_number();
  static const int kSequenceNumberFieldNumber = 6;
  inline ::google::protobuf::uint64 sequence_number() const;
  inline void set_sequence_number(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:base.log.LogSegmentHeaderPB)
 private:
  inline void set_has_major_version();
  inline void clear_has_major_version();
  inline void set_has_minor_version();
  inline void clear_has_minor_version();
  inline void set_has_tablet_id();
  inline void clear_has_tablet_id();
  inline void set_has_sequence_number();
  inline void clear_has_sequence_number();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 major_version_;
  ::google::protobuf::uint32 minor_version_;
  ::std::string* tablet_id_;
  ::google::protobuf::uint64 sequence_number_;
  friend void  protobuf_AddDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_AssignDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_ShutdownFile_base_2fraft_2fproto_2flog_2eproto();

  void InitAsDefaultInstance();
  static LogSegmentHeaderPB* default_instance_;
};
// -------------------------------------------------------------------

class LogSegmentFooterPB : public ::google::protobuf::Message {
 public:
  LogSegmentFooterPB();
  virtual ~LogSegmentFooterPB();

  LogSegmentFooterPB(const LogSegmentFooterPB& from);

  inline LogSegmentFooterPB& operator=(const LogSegmentFooterPB& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LogSegmentFooterPB& default_instance();

  void Swap(LogSegmentFooterPB* other);

  // implements Message ----------------------------------------------

  LogSegmentFooterPB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LogSegmentFooterPB& from);
  void MergeFrom(const LogSegmentFooterPB& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int64 num_entries = 1;
  inline bool has_num_entries() const;
  inline void clear_num_entries();
  static const int kNumEntriesFieldNumber = 1;
  inline ::google::protobuf::int64 num_entries() const;
  inline void set_num_entries(::google::protobuf::int64 value);

  // optional int64 min_replicate_index = 2 [default = -1];
  inline bool has_min_replicate_index() const;
  inline void clear_min_replicate_index();
  static const int kMinReplicateIndexFieldNumber = 2;
  inline ::google::protobuf::int64 min_replicate_index() const;
  inline void set_min_replicate_index(::google::protobuf::int64 value);

  // optional int64 max_replicate_index = 3 [default = -1];
  inline bool has_max_replicate_index() const;
  inline void clear_max_replicate_index();
  static const int kMaxReplicateIndexFieldNumber = 3;
  inline ::google::protobuf::int64 max_replicate_index() const;
  inline void set_max_replicate_index(::google::protobuf::int64 value);

  // optional int64 close_timestamp_micros = 4;
  inline bool has_close_timestamp_micros() const;
  inline void clear_close_timestamp_micros();
  static const int kCloseTimestampMicrosFieldNumber = 4;
  inline ::google::protobuf::int64 close_timestamp_micros() const;
  inline void set_close_timestamp_micros(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:base.log.LogSegmentFooterPB)
 private:
  inline void set_has_num_entries();
  inline void clear_has_num_entries();
  inline void set_has_min_replicate_index();
  inline void clear_has_min_replicate_index();
  inline void set_has_max_replicate_index();
  inline void clear_has_max_replicate_index();
  inline void set_has_close_timestamp_micros();
  inline void clear_has_close_timestamp_micros();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int64 num_entries_;
  ::google::protobuf::int64 min_replicate_index_;
  ::google::protobuf::int64 max_replicate_index_;
  ::google::protobuf::int64 close_timestamp_micros_;
  friend void  protobuf_AddDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_AssignDesc_base_2fraft_2fproto_2flog_2eproto();
  friend void protobuf_ShutdownFile_base_2fraft_2fproto_2flog_2eproto();

  void InitAsDefaultInstance();
  static LogSegmentFooterPB* default_instance_;
};
// ===================================================================


// ===================================================================

// LogEntryPB

// required .base.log.LogEntryTypePB type = 1;
inline bool LogEntryPB::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LogEntryPB::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LogEntryPB::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LogEntryPB::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::base::log::LogEntryTypePB LogEntryPB::type() const {
  // @@protoc_insertion_point(field_get:base.log.LogEntryPB.type)
  return static_cast< ::base::log::LogEntryTypePB >(type_);
}
inline void LogEntryPB::set_type(::base::log::LogEntryTypePB value) {
  assert(::base::log::LogEntryTypePB_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogEntryPB.type)
}

// optional .base.consensus.ReplicateMsg replicate = 2;
inline bool LogEntryPB::has_replicate() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LogEntryPB::set_has_replicate() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LogEntryPB::clear_has_replicate() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LogEntryPB::clear_replicate() {
  if (replicate_ != NULL) replicate_->::base::consensus::ReplicateMsg::Clear();
  clear_has_replicate();
}
inline const ::base::consensus::ReplicateMsg& LogEntryPB::replicate() const {
  // @@protoc_insertion_point(field_get:base.log.LogEntryPB.replicate)
  return replicate_ != NULL ? *replicate_ : *default_instance_->replicate_;
}
inline ::base::consensus::ReplicateMsg* LogEntryPB::mutable_replicate() {
  set_has_replicate();
  if (replicate_ == NULL) replicate_ = new ::base::consensus::ReplicateMsg;
  // @@protoc_insertion_point(field_mutable:base.log.LogEntryPB.replicate)
  return replicate_;
}
inline ::base::consensus::ReplicateMsg* LogEntryPB::release_replicate() {
  clear_has_replicate();
  ::base::consensus::ReplicateMsg* temp = replicate_;
  replicate_ = NULL;
  return temp;
}
inline void LogEntryPB::set_allocated_replicate(::base::consensus::ReplicateMsg* replicate) {
  delete replicate_;
  replicate_ = replicate;
  if (replicate) {
    set_has_replicate();
  } else {
    clear_has_replicate();
  }
  // @@protoc_insertion_point(field_set_allocated:base.log.LogEntryPB.replicate)
}

// optional .base.consensus.CommitMsg commit = 3;
inline bool LogEntryPB::has_commit() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void LogEntryPB::set_has_commit() {
  _has_bits_[0] |= 0x00000004u;
}
inline void LogEntryPB::clear_has_commit() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void LogEntryPB::clear_commit() {
  if (commit_ != NULL) commit_->::base::consensus::CommitMsg::Clear();
  clear_has_commit();
}
inline const ::base::consensus::CommitMsg& LogEntryPB::commit() const {
  // @@protoc_insertion_point(field_get:base.log.LogEntryPB.commit)
  return commit_ != NULL ? *commit_ : *default_instance_->commit_;
}
inline ::base::consensus::CommitMsg* LogEntryPB::mutable_commit() {
  set_has_commit();
  if (commit_ == NULL) commit_ = new ::base::consensus::CommitMsg;
  // @@protoc_insertion_point(field_mutable:base.log.LogEntryPB.commit)
  return commit_;
}
inline ::base::consensus::CommitMsg* LogEntryPB::release_commit() {
  clear_has_commit();
  ::base::consensus::CommitMsg* temp = commit_;
  commit_ = NULL;
  return temp;
}
inline void LogEntryPB::set_allocated_commit(::base::consensus::CommitMsg* commit) {
  delete commit_;
  commit_ = commit;
  if (commit) {
    set_has_commit();
  } else {
    clear_has_commit();
  }
  // @@protoc_insertion_point(field_set_allocated:base.log.LogEntryPB.commit)
}

// -------------------------------------------------------------------

// LogEntryBatchPB

// repeated .base.log.LogEntryPB entry = 1;
inline int LogEntryBatchPB::entry_size() const {
  return entry_.size();
}
inline void LogEntryBatchPB::clear_entry() {
  entry_.Clear();
}
inline const ::base::log::LogEntryPB& LogEntryBatchPB::entry(int index) const {
  // @@protoc_insertion_point(field_get:base.log.LogEntryBatchPB.entry)
  return entry_.Get(index);
}
inline ::base::log::LogEntryPB* LogEntryBatchPB::mutable_entry(int index) {
  // @@protoc_insertion_point(field_mutable:base.log.LogEntryBatchPB.entry)
  return entry_.Mutable(index);
}
inline ::base::log::LogEntryPB* LogEntryBatchPB::add_entry() {
  // @@protoc_insertion_point(field_add:base.log.LogEntryBatchPB.entry)
  return entry_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::base::log::LogEntryPB >&
LogEntryBatchPB::entry() const {
  // @@protoc_insertion_point(field_list:base.log.LogEntryBatchPB.entry)
  return entry_;
}
inline ::google::protobuf::RepeatedPtrField< ::base::log::LogEntryPB >*
LogEntryBatchPB::mutable_entry() {
  // @@protoc_insertion_point(field_mutable_list:base.log.LogEntryBatchPB.entry)
  return &entry_;
}

// -------------------------------------------------------------------

// LogSegmentHeaderPB

// required uint32 major_version = 1;
inline bool LogSegmentHeaderPB::has_major_version() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LogSegmentHeaderPB::set_has_major_version() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LogSegmentHeaderPB::clear_has_major_version() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LogSegmentHeaderPB::clear_major_version() {
  major_version_ = 0u;
  clear_has_major_version();
}
inline ::google::protobuf::uint32 LogSegmentHeaderPB::major_version() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentHeaderPB.major_version)
  return major_version_;
}
inline void LogSegmentHeaderPB::set_major_version(::google::protobuf::uint32 value) {
  set_has_major_version();
  major_version_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogSegmentHeaderPB.major_version)
}

// required uint32 minor_version = 2;
inline bool LogSegmentHeaderPB::has_minor_version() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LogSegmentHeaderPB::set_has_minor_version() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LogSegmentHeaderPB::clear_has_minor_version() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LogSegmentHeaderPB::clear_minor_version() {
  minor_version_ = 0u;
  clear_has_minor_version();
}
inline ::google::protobuf::uint32 LogSegmentHeaderPB::minor_version() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentHeaderPB.minor_version)
  return minor_version_;
}
inline void LogSegmentHeaderPB::set_minor_version(::google::protobuf::uint32 value) {
  set_has_minor_version();
  minor_version_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogSegmentHeaderPB.minor_version)
}

// required bytes tablet_id = 5;
inline bool LogSegmentHeaderPB::has_tablet_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void LogSegmentHeaderPB::set_has_tablet_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void LogSegmentHeaderPB::clear_has_tablet_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void LogSegmentHeaderPB::clear_tablet_id() {
  if (tablet_id_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    tablet_id_->clear();
  }
  clear_has_tablet_id();
}
inline const ::std::string& LogSegmentHeaderPB::tablet_id() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentHeaderPB.tablet_id)
  return *tablet_id_;
}
inline void LogSegmentHeaderPB::set_tablet_id(const ::std::string& value) {
  set_has_tablet_id();
  if (tablet_id_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    tablet_id_ = new ::std::string;
  }
  tablet_id_->assign(value);
  // @@protoc_insertion_point(field_set:base.log.LogSegmentHeaderPB.tablet_id)
}
inline void LogSegmentHeaderPB::set_tablet_id(const char* value) {
  set_has_tablet_id();
  if (tablet_id_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    tablet_id_ = new ::std::string;
  }
  tablet_id_->assign(value);
  // @@protoc_insertion_point(field_set_char:base.log.LogSegmentHeaderPB.tablet_id)
}
inline void LogSegmentHeaderPB::set_tablet_id(const void* value, size_t size) {
  set_has_tablet_id();
  if (tablet_id_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    tablet_id_ = new ::std::string;
  }
  tablet_id_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:base.log.LogSegmentHeaderPB.tablet_id)
}
inline ::std::string* LogSegmentHeaderPB::mutable_tablet_id() {
  set_has_tablet_id();
  if (tablet_id_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    tablet_id_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:base.log.LogSegmentHeaderPB.tablet_id)
  return tablet_id_;
}
inline ::std::string* LogSegmentHeaderPB::release_tablet_id() {
  clear_has_tablet_id();
  if (tablet_id_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = tablet_id_;
    tablet_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void LogSegmentHeaderPB::set_allocated_tablet_id(::std::string* tablet_id) {
  if (tablet_id_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete tablet_id_;
  }
  if (tablet_id) {
    set_has_tablet_id();
    tablet_id_ = tablet_id;
  } else {
    clear_has_tablet_id();
    tablet_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:base.log.LogSegmentHeaderPB.tablet_id)
}

// required uint64 sequence_number = 6;
inline bool LogSegmentHeaderPB::has_sequence_number() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void LogSegmentHeaderPB::set_has_sequence_number() {
  _has_bits_[0] |= 0x00000008u;
}
inline void LogSegmentHeaderPB::clear_has_sequence_number() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void LogSegmentHeaderPB::clear_sequence_number() {
  sequence_number_ = GOOGLE_ULONGLONG(0);
  clear_has_sequence_number();
}
inline ::google::protobuf::uint64 LogSegmentHeaderPB::sequence_number() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentHeaderPB.sequence_number)
  return sequence_number_;
}
inline void LogSegmentHeaderPB::set_sequence_number(::google::protobuf::uint64 value) {
  set_has_sequence_number();
  sequence_number_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogSegmentHeaderPB.sequence_number)
}

// -------------------------------------------------------------------

// LogSegmentFooterPB

// required int64 num_entries = 1;
inline bool LogSegmentFooterPB::has_num_entries() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LogSegmentFooterPB::set_has_num_entries() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LogSegmentFooterPB::clear_has_num_entries() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LogSegmentFooterPB::clear_num_entries() {
  num_entries_ = GOOGLE_LONGLONG(0);
  clear_has_num_entries();
}
inline ::google::protobuf::int64 LogSegmentFooterPB::num_entries() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentFooterPB.num_entries)
  return num_entries_;
}
inline void LogSegmentFooterPB::set_num_entries(::google::protobuf::int64 value) {
  set_has_num_entries();
  num_entries_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogSegmentFooterPB.num_entries)
}

// optional int64 min_replicate_index = 2 [default = -1];
inline bool LogSegmentFooterPB::has_min_replicate_index() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LogSegmentFooterPB::set_has_min_replicate_index() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LogSegmentFooterPB::clear_has_min_replicate_index() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LogSegmentFooterPB::clear_min_replicate_index() {
  min_replicate_index_ = GOOGLE_LONGLONG(-1);
  clear_has_min_replicate_index();
}
inline ::google::protobuf::int64 LogSegmentFooterPB::min_replicate_index() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentFooterPB.min_replicate_index)
  return min_replicate_index_;
}
inline void LogSegmentFooterPB::set_min_replicate_index(::google::protobuf::int64 value) {
  set_has_min_replicate_index();
  min_replicate_index_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogSegmentFooterPB.min_replicate_index)
}

// optional int64 max_replicate_index = 3 [default = -1];
inline bool LogSegmentFooterPB::has_max_replicate_index() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void LogSegmentFooterPB::set_has_max_replicate_index() {
  _has_bits_[0] |= 0x00000004u;
}
inline void LogSegmentFooterPB::clear_has_max_replicate_index() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void LogSegmentFooterPB::clear_max_replicate_index() {
  max_replicate_index_ = GOOGLE_LONGLONG(-1);
  clear_has_max_replicate_index();
}
inline ::google::protobuf::int64 LogSegmentFooterPB::max_replicate_index() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentFooterPB.max_replicate_index)
  return max_replicate_index_;
}
inline void LogSegmentFooterPB::set_max_replicate_index(::google::protobuf::int64 value) {
  set_has_max_replicate_index();
  max_replicate_index_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogSegmentFooterPB.max_replicate_index)
}

// optional int64 close_timestamp_micros = 4;
inline bool LogSegmentFooterPB::has_close_timestamp_micros() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void LogSegmentFooterPB::set_has_close_timestamp_micros() {
  _has_bits_[0] |= 0x00000008u;
}
inline void LogSegmentFooterPB::clear_has_close_timestamp_micros() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void LogSegmentFooterPB::clear_close_timestamp_micros() {
  close_timestamp_micros_ = GOOGLE_LONGLONG(0);
  clear_has_close_timestamp_micros();
}
inline ::google::protobuf::int64 LogSegmentFooterPB::close_timestamp_micros() const {
  // @@protoc_insertion_point(field_get:base.log.LogSegmentFooterPB.close_timestamp_micros)
  return close_timestamp_micros_;
}
inline void LogSegmentFooterPB::set_close_timestamp_micros(::google::protobuf::int64 value) {
  set_has_close_timestamp_micros();
  close_timestamp_micros_ = value;
  // @@protoc_insertion_point(field_set:base.log.LogSegmentFooterPB.close_timestamp_micros)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace log
}  // namespace base

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::base::log::LogEntryTypePB> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::base::log::LogEntryTypePB>() {
  return ::base::log::LogEntryTypePB_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_base_2fraft_2fproto_2flog_2eproto__INCLUDED
