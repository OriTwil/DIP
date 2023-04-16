// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: cartographer/mapping_3d/proto/motion_filter_options.proto

#ifndef PROTOBUF_cartographer_2fmapping_5f3d_2fproto_2fmotion_5ffilter_5foptions_2eproto__INCLUDED
#define PROTOBUF_cartographer_2fmapping_5f3d_2fproto_2fmotion_5ffilter_5foptions_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace cartographer {
namespace mapping_3d {
namespace proto {
class MotionFilterOptions;
class MotionFilterOptionsDefaultTypeInternal;
extern MotionFilterOptionsDefaultTypeInternal _MotionFilterOptions_default_instance_;
}  // namespace proto
}  // namespace mapping_3d
}  // namespace cartographer

namespace cartographer {
namespace mapping_3d {
namespace proto {

namespace protobuf_cartographer_2fmapping_5f3d_2fproto_2fmotion_5ffilter_5foptions_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static void InitDefaultsImpl();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_cartographer_2fmapping_5f3d_2fproto_2fmotion_5ffilter_5foptions_2eproto

// ===================================================================

class MotionFilterOptions : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:cartographer.mapping_3d.proto.MotionFilterOptions) */ {
 public:
  MotionFilterOptions();
  virtual ~MotionFilterOptions();

  MotionFilterOptions(const MotionFilterOptions& from);

  inline MotionFilterOptions& operator=(const MotionFilterOptions& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  MotionFilterOptions(MotionFilterOptions&& from) noexcept
    : MotionFilterOptions() {
    *this = ::std::move(from);
  }

  inline MotionFilterOptions& operator=(MotionFilterOptions&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const MotionFilterOptions& default_instance();

  static inline const MotionFilterOptions* internal_default_instance() {
    return reinterpret_cast<const MotionFilterOptions*>(
               &_MotionFilterOptions_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(MotionFilterOptions* other);
  friend void swap(MotionFilterOptions& a, MotionFilterOptions& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline MotionFilterOptions* New() const PROTOBUF_FINAL { return New(NULL); }

  MotionFilterOptions* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const MotionFilterOptions& from);
  void MergeFrom(const MotionFilterOptions& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(MotionFilterOptions* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // double max_time_seconds = 1;
  void clear_max_time_seconds();
  static const int kMaxTimeSecondsFieldNumber = 1;
  double max_time_seconds() const;
  void set_max_time_seconds(double value);

  // double max_distance_meters = 2;
  void clear_max_distance_meters();
  static const int kMaxDistanceMetersFieldNumber = 2;
  double max_distance_meters() const;
  void set_max_distance_meters(double value);

  // double max_angle_radians = 3;
  void clear_max_angle_radians();
  static const int kMaxAngleRadiansFieldNumber = 3;
  double max_angle_radians() const;
  void set_max_angle_radians(double value);

  // @@protoc_insertion_point(class_scope:cartographer.mapping_3d.proto.MotionFilterOptions)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  double max_time_seconds_;
  double max_distance_meters_;
  double max_angle_radians_;
  mutable int _cached_size_;
  friend struct protobuf_cartographer_2fmapping_5f3d_2fproto_2fmotion_5ffilter_5foptions_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// MotionFilterOptions

// double max_time_seconds = 1;
inline void MotionFilterOptions::clear_max_time_seconds() {
  max_time_seconds_ = 0;
}
inline double MotionFilterOptions::max_time_seconds() const {
  // @@protoc_insertion_point(field_get:cartographer.mapping_3d.proto.MotionFilterOptions.max_time_seconds)
  return max_time_seconds_;
}
inline void MotionFilterOptions::set_max_time_seconds(double value) {
  
  max_time_seconds_ = value;
  // @@protoc_insertion_point(field_set:cartographer.mapping_3d.proto.MotionFilterOptions.max_time_seconds)
}

// double max_distance_meters = 2;
inline void MotionFilterOptions::clear_max_distance_meters() {
  max_distance_meters_ = 0;
}
inline double MotionFilterOptions::max_distance_meters() const {
  // @@protoc_insertion_point(field_get:cartographer.mapping_3d.proto.MotionFilterOptions.max_distance_meters)
  return max_distance_meters_;
}
inline void MotionFilterOptions::set_max_distance_meters(double value) {
  
  max_distance_meters_ = value;
  // @@protoc_insertion_point(field_set:cartographer.mapping_3d.proto.MotionFilterOptions.max_distance_meters)
}

// double max_angle_radians = 3;
inline void MotionFilterOptions::clear_max_angle_radians() {
  max_angle_radians_ = 0;
}
inline double MotionFilterOptions::max_angle_radians() const {
  // @@protoc_insertion_point(field_get:cartographer.mapping_3d.proto.MotionFilterOptions.max_angle_radians)
  return max_angle_radians_;
}
inline void MotionFilterOptions::set_max_angle_radians(double value) {
  
  max_angle_radians_ = value;
  // @@protoc_insertion_point(field_set:cartographer.mapping_3d.proto.MotionFilterOptions.max_angle_radians)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace proto
}  // namespace mapping_3d
}  // namespace cartographer

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_cartographer_2fmapping_5f3d_2fproto_2fmotion_5ffilter_5foptions_2eproto__INCLUDED
