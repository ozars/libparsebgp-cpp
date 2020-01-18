#pragma once

#include <cstdint>

#include <parsebgp/bgp/common.hpp>
#include <parsebgp/utils.hpp>

extern "C" {
struct parsebgp_bgp_update_as_path_seg;
struct parsebgp_bgp_update_as_path;
struct parsebgp_bgp_update_aggregator;
struct parsebgp_bgp_update_communities;
struct parsebgp_bgp_update_cluster_list;
struct parsebgp_bgp_update_as_pathlimit;
struct parsebgp_bgp_update_large_community;
struct parsebgp_bgp_update_large_communities;
struct parsebgp_bgp_update_path_attr;
struct parsebgp_bgp_update_path_attrs;
struct parsebgp_bgp_update_nlris;
struct parsebgp_bgp_update;
}

namespace parsebgp {
namespace bgp {

class PathAttributes : public utils::CPtrView<PathAttributes, parsebgp_bgp_update_path_attrs*> {
public:
  class Flags : public utils::FlagsClass<Flags> {
  public:
    enum Mask {
      OPTIONAL = 0x80,
      TRANSITIVE = 0x40,
      PARTIAL = 0x20,
      EXTENDED = 0x10,
    };

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    Flags(uint8_t value) : value_(value) {}

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    operator uint8_t() const { return value_; }

    uint8_t value() const { return value_; }
    bool is_optional() const { return value_ & OPTIONAL; }
    bool is_transitive() const { return value_ & TRANSITIVE; }
    bool is_partial() const { return value_ & PARTIAL; }
    bool is_extended() const { return value_ & EXTENDED; }

  private:
    uint8_t value_;
  };

  class Type : public utils::EnumClass<Type> {
  public:
    enum Value {
      ORIGIN = 1,
      AS_PATH = 2,
      NEXT_HOP = 3,
      MED = 4,
      LOCAL_PREF = 5,
      ATOMIC_AGGREGATE = 6,
      AGGREGATOR = 7,
      COMMUNITIES = 8,
      ORIGINATOR_ID = 9,
      CLUSTER_LIST = 10,
      MP_REACH_NLRI = 14,
      MP_UNREACH_NLRI = 15,
      EXT_COMMUNITIES = 16,
      AS4_PATH = 17,
      AS4_AGGREGATOR = 18,
      AS_PATHLIMIT = 21,
      IPV6_EXT_COMMUNITIES = 25,
      BGP_LS = 29,
      LARGE_COMMUNITIES = 32,
    };

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    Type(Value value) : value_(value) {}

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    operator Value() const { return value_; }

    Value value() const { return value_; }
    bool is_valid() const {
      switch (value_) {
        case ORIGIN:
        case AS_PATH:
        case NEXT_HOP:
        case MED:
        case LOCAL_PREF:
        case ATOMIC_AGGREGATE:
        case AGGREGATOR:
        case COMMUNITIES:
        case ORIGINATOR_ID:
        case CLUSTER_LIST:
        case MP_REACH_NLRI:
        case MP_UNREACH_NLRI:
        case EXT_COMMUNITIES:
        case AS4_PATH:
        case AS4_AGGREGATOR:
        case AS_PATHLIMIT:
        case IPV6_EXT_COMMUNITIES:
        case BGP_LS:
        case LARGE_COMMUNITIES:
          return true;
      }
      return false;
    }
    bool is_origin() const { return value_ == ORIGIN; }
    bool is_as_path() const { return value_ == AS_PATH; }
    bool is_next_hop() const { return value_ == NEXT_HOP; }
    bool is_med() const { return value_ == MED; }
    bool is_local_pref() const { return value_ == LOCAL_PREF; }
    bool is_atomic_aggregate() const { return value_ == ATOMIC_AGGREGATE; }
    bool is_aggregator() const { return value_ == AGGREGATOR; }
    bool is_communities() const { return value_ == COMMUNITIES; }
    bool is_originator_id() const { return value_ == ORIGINATOR_ID; }
    bool is_cluster_list() const { return value_ == CLUSTER_LIST; }
    bool is_mp_reach_nlri() const { return value_ == MP_REACH_NLRI; }
    bool is_mp_unreach_nlri() const { return value_ == MP_UNREACH_NLRI; }
    bool is_ext_communities() const { return value_ == EXT_COMMUNITIES; }
    bool is_as4_path() const { return value_ == AS4_PATH; }
    bool is_as4_aggregator() const { return value_ == AS4_AGGREGATOR; }
    bool is_as_pathlimit() const { return value_ == AS_PATHLIMIT; }
    bool is_ipv6_ext_communities() const { return value_ == IPV6_EXT_COMMUNITIES; }
    bool is_bgp_ls() const { return value_ == BGP_LS; }
    bool is_large_communities() const { return value_ == LARGE_COMMUNITIES; }

  private:
    Value value_;
  };

private:
  class Base : public utils::CPtrView<Base, parsebgp_bgp_update_path_attr*> {
  public:
    // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
    Base(CPtr cptr) : BaseView(cptr) {}

    Flags flags() const;
    Type type() const;

  protected:
    using BaseView::cptr;
  };

#define PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(ClassName_, ValueType_)                                \
  class ClassName_ : private Base {                                                                \
  public:                                                                                          \
    using ValueType = ValueType_;                                                                  \
                                                                                                   \
    using Base::Base;                                                                              \
    using Base::flags;                                                                             \
    using Base::type;                                                                              \
    ValueType value() const;                                                                       \
    operator ValueType() const;                                                                    \
  }

public:
  PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(Origin, uint8_t);
  PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(NextHop, const utils::ipv4_view);
  PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(Med, uint32_t);
  PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(LocalPref, uint32_t);
  PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(OriginatorId, uint32_t);

#undef PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE

  class AsPathSegment
    : public utils::CPtrView<AsPathSegment, parsebgp_bgp_update_as_path_seg*>
    , public utils::CPtrRange<AsPathSegment, Asn, uint32_t*> {
  public:
    class Type : public utils::EnumClass<Type> {
    public:
      enum Value : uint8_t {
        AS_SET = 1,
        AS_SEQ = 2,
        CONFED_SEQ = 3,
        CONFED_SET = 4,
      };

      // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
      Type(Value value) : value_(value) {}

      // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
      operator Value() const { return value_; }

      Value value() const { return value_; }
      bool is_valid() const {
        switch (value_) {
          case AS_SET:
          case AS_SEQ:
          case CONFED_SEQ:
          case CONFED_SET:
            return true;
        }
        return false;
      }
      bool is_as_set() const { return value_ == AS_SET; }
      bool is_as_seq() const { return value_ == AS_SEQ; }
      bool is_confed_seq() const { return value_ == CONFED_SEQ; }
      bool is_confed_set() const { return value_ == CONFED_SET; }

    private:
      Value value_;
    };

    // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
    AsPathSegment(CPtr cptr) : BaseView(cptr) {}

    Type type() const;

  private:
    friend BaseRange;
    ElementCPtr range_data() const;
    std::size_t range_size() const;
    static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n) { return ptr + n; }
    static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n) { return ptr - n; }
    static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs) {
      return lhs - rhs;
    }
  };

  class AsPath
    : private Base
    , public utils::CPtrRange<AsPath, AsPathSegment> {
  public:
    using Base::Base;
    using Base::flags;
    using Base::type;

    uint8_t asns_count() const;
    bool asn_4_byte() const;

  private:
    friend BaseRange;
    ElementCPtr range_data() const;
    std::size_t range_size() const;
    static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n);
    static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n);
    static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs);
  };

  class Aggregator : private Base {
  public:
    using Base::Base;
    using Base::flags;
    using Base::type;
    uint32_t asn() const;
    utils::ipv4_view addr() const;
  };

  union Community {
    uint32_t u32;
    struct {
      // FIXME: assumes little endian
      uint16_t value;
      uint16_t asn;
    };
    // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
    Community(uint32_t* cptr) : u32(*cptr) {}
  };

  class Communities
    : private Base
    , public utils::CPtrRange<Communities, Community, uint32_t*> {
  public:
    using Base::Base;
    using Base::flags;
    using Base::type;

  private:
    friend BaseRange;
    ElementCPtr range_data() const;
    std::size_t range_size() const;
    static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n) { return ptr + n; }
    static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n) { return ptr - n; }
    static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs) {
      return lhs - rhs;
    }
  };

  class ClusterList
    : private Base
    , public utils::CPtrRange<ClusterList, uint32_t, uint32_t*> {
  public:
    using Base::Base;
    using Base::flags;
    using Base::type;

  private:
    friend BaseRange;
    ElementCPtr range_data() const;
    std::size_t range_size() const;
    static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n) { return ptr + n; }
    static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n) { return ptr - n; }
    static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs) {
      return lhs - rhs;
    }
  };

  class MpReach
    : public Base
    , public utils::CPtrRange<MpReach, Prefix> {
  public:
    using Base::Base;
    using Base::flags;
    using Base::type;

    AfiType afi_type() const;
    SafiType safi_type() const;
    utils::bytes_view raw_next_hop() const;
    utils::ipv4_view next_hop_ipv4() const;
    utils::ipv6_view next_hop_ipv6() const;

  private:
    friend BaseRange;
    ElementCPtr range_data() const;
    std::size_t range_size() const;
    static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n);
    static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n);
    static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs);
  };

  class MpUnreach
    : public Base
    , public utils::CPtrRange<MpUnreach, Prefix> {
  public:
    using Base::Base;
    using Base::flags;
    using Base::type;

    AfiType afi_type() const;
    SafiType safi_type() const;

  private:
    friend BaseRange;
    ElementCPtr range_data() const;
    std::size_t range_size() const;
    static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n);
    static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n);
    static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs);
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  PathAttributes(CPtr cptr) : BaseView(cptr) {}

  bool has_origin() const;
  Origin origin() const;

  bool has_as_path() const;
  AsPath as_path() const;

  bool has_next_hop() const;
  NextHop next_hop() const;

  bool has_med() const;
  Med med() const;

  bool has_local_pref() const;
  LocalPref local_pref() const;

  bool has_atomic_aggregate() const;

  bool has_aggregator() const;
  Aggregator aggregator() const;

  bool has_communities() const;
  Communities communities() const;

  bool has_originator_id() const;
  OriginatorId originator_id() const;

  bool has_cluster_list() const;
  ClusterList cluster_list() const;

  bool has_mp_reach() const;
  MpReach mp_reach() const;

  bool has_mp_unreach() const;
  MpUnreach mp_unreach() const;

  // bool has_ext_communities() const;
  // ExtCommunities ext_communities() const;

  // bool has_as4_path() const;
  // AsPath as4_path() const;

  // bool has_as4_aggregator() const;
  // Aggregator as4_aggregator() const;

  // bool has_as_pathlimit() const;
  // AsPathlimit as_pathlimit() const;

  // bool has_ipv6_ext_communities() const;
  // ExtCommunitiesIpv6 bool ipv6_ext_communities() const;

  // bool has_bgp_ls() const;
  // BgpLs bgp_ls() const;

  // bool has_large_communities() const;
  // LargeCommunities large_communities() const;

  // TODO: Rest of path attributes
};

} // namespace bgp
} // namespace parsebgp
