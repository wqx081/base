// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: base/util/pb_util.proto

#ifndef PROTOBUF_base_2futil_2fpb_5futil_2eproto__INCLUDED
#define PROTOBUF_base_2futil_2fpb_5futil_2eproto__INCLUDED

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
#include <google/protobuf/unknown_field_set.h>
#include "google/protobuf/descriptor.pb.h"
// @@protoc_insertion_point(includes)

namespace base {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_base_2futil_2fpb_5futil_2eproto();
void protobuf_AssignDesc_base_2futil_2fpb_5futil_2eproto();
void protobuf_ShutdownFile_base_2futil_2fpb_5futil_2eproto();

class ContainerSupHeaderPB;

// ===================================================================

class ContainerSupHeaderPB : public ::google::protobuf::Message {
 public:
  ContainerSupHeaderPB();
  virtual ~ContainerSupHeaderPB();

  ContainerSupHeaderPB(const ContainerSupHeaderPB& from);

  inline ContainerSupHeaderPB& operator=(const ContainerSupHeaderPB& from) {
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
  static const ContainerSupHeaderPB& default_instance();

  void Swap(ContainerSupHeaderPB* other);

  // implements Message ----------------------------------------------

  ContainerSupHeaderPB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ContainerSupHeaderPB& from);
  void MergeFrom(const ContainerSupHeaderPB& from);
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

  // required .google.protobuf.FileDescriptorSet protos = 1;
  inline bool has_protos() const;
  inline void clear_protos();
  static const int kProtosFieldNumber = 1;
  inline const ::google::protobuf::FileDescriptorSet& protos() const;
  inline ::google::protobuf::FileDescriptorSet* mutable_protos();
  inline ::google::protobuf::FileDescriptorSet* release_protos();
  inline void set_allocated_protos(::google::protobuf::FileDescriptorSet* protos);

  // required string pb_type = 2;
  inline bool has_pb_type() const;
  inline void clear_pb_type();
  static const int kPbTypeFieldNumber = 2;
  inline const ::std::string& pb_type() const;
  inline void set_pb_type(const ::std::string& value);
  inline void set_pb_type(const char* value);
  inline void set_pb_type(const char* value, size_t size);
  inline ::std::string* mutable_pb_type();
  inline ::std::string* release_pb_type();
  inline void set_allocated_pb_type(::std::string* pb_type);

  // @@protoc_insertion_point(class_scope:base.ContainerSupHeaderPB)
 private:
  inline void set_has_protos();
  inline void clear_has_protos();
  inline void set_has_pb_type();
  inline void clear_has_pb_type();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::FileDescriptorSet* protos_;
  ::std::string* pb_type_;
  friend void  protobuf_AddDesc_base_2futil_2fpb_5futil_2eproto();
  friend void protobuf_AssignDesc_base_2futil_2fpb_5futil_2eproto();
  friend void protobuf_ShutdownFile_base_2futil_2fpb_5futil_2eproto();

  void InitAsDefaultInstance();
  static ContainerSupHeaderPB* default_instance_;
};
// ===================================================================

static const int kREDACTFieldNumber = 50001;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::FieldOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< bool >, 8, false >
  REDACT;

// ===================================================================

// ContainerSupHeaderPB

// required .google.protobuf.FileDescriptorSet protos = 1;
inline bool ContainerSupHeaderPB::has_protos() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ContainerSupHeaderPB::set_has_protos() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ContainerSupHeaderPB::clear_has_protos() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ContainerSupHeaderPB::clear_protos() {
  if (protos_ != NULL) protos_->::google::protobuf::FileDescriptorSet::Clear();
  clear_has_protos();
}
inline const ::google::protobuf::FileDescriptorSet& ContainerSupHeaderPB::protos() const {
  // @@protoc_insertion_point(field_get:base.ContainerSupHeaderPB.protos)
  return protos_ != NULL ? *protos_ : *default_instance_->protos_;
}
inline ::google::protobuf::FileDescriptorSet* ContainerSupHeaderPB::mutable_protos() {
  set_has_protos();
  if (protos_ == NULL) protos_ = new ::google::protobuf::FileDescriptorSet;
  // @@protoc_insertion_point(field_mutable:base.ContainerSupHeaderPB.protos)
  return protos_;
}
inline ::google::protobuf::FileDescriptorSet* ContainerSupHeaderPB::release_protos() {
  clear_has_protos();
  ::google::protobuf::FileDescriptorSet* temp = protos_;
  protos_ = NULL;
  return temp;
}
inline void ContainerSupHeaderPB::set_allocated_protos(::google::protobuf::FileDescriptorSet* protos) {
  delete protos_;
  protos_ = protos;
  if (protos) {
    set_has_protos();
  } else {
    clear_has_protos();
  }
  // @@protoc_insertion_point(field_set_allocated:base.ContainerSupHeaderPB.protos)
}

// required string pb_type = 2;
inline bool ContainerSupHeaderPB::has_pb_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ContainerSupHeaderPB::set_has_pb_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ContainerSupHeaderPB::clear_has_pb_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ContainerSupHeaderPB::clear_pb_type() {
  if (pb_type_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    pb_type_->clear();
  }
  clear_has_pb_type();
}
inline const ::std::string& ContainerSupHeaderPB::pb_type() const {
  // @@protoc_insertion_point(field_get:base.ContainerSupHeaderPB.pb_type)
  return *pb_type_;
}
inline void ContainerSupHeaderPB::set_pb_type(const ::std::string& value) {
  set_has_pb_type();
  if (pb_type_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    pb_type_ = new ::std::string;
  }
  pb_type_->assign(value);
  // @@protoc_insertion_point(field_set:base.ContainerSupHeaderPB.pb_type)
}
inline void ContainerSupHeaderPB::set_pb_type(const char* value) {
  set_has_pb_type();
  if (pb_type_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    pb_type_ = new ::std::string;
  }
  pb_type_->assign(value);
  // @@protoc_insertion_point(field_set_char:base.ContainerSupHeaderPB.pb_type)
}
inline void ContainerSupHeaderPB::set_pb_type(const char* value, size_t size) {
  set_has_pb_type();
  if (pb_type_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    pb_type_ = new ::std::string;
  }
  pb_type_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:base.ContainerSupHeaderPB.pb_type)
}
inline ::std::string* ContainerSupHeaderPB::mutable_pb_type() {
  set_has_pb_type();
  if (pb_type_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    pb_type_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:base.ContainerSupHeaderPB.pb_type)
  return pb_type_;
}
inline ::std::string* ContainerSupHeaderPB::release_pb_type() {
  clear_has_pb_type();
  if (pb_type_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = pb_type_;
    pb_type_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ContainerSupHeaderPB::set_allocated_pb_type(::std::string* pb_type) {
  if (pb_type_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete pb_type_;
  }
  if (pb_type) {
    set_has_pb_type();
    pb_type_ = pb_type;
  } else {
    clear_has_pb_type();
    pb_type_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:base.ContainerSupHeaderPB.pb_type)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace base

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_base_2futil_2fpb_5futil_2eproto__INCLUDED
