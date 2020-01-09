#pragma once

#include <cstdint>

#include "parsebgp/utils.hpp"

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

class AfiType : public utils::EnumClass<AfiType> {
public:
  enum Value {
    IPV4 = 1,
    IPV6 = 2,
  };

  Value value() const { return value_; }
  bool is_valid() const { return value_ == IPV4 || value_ == IPV6; }
  bool is_ipv4() const { return value_ == IPV4; }
  bool is_ipv6() const { return value_ == IPV6; }

private:
  AfiType(Value value) : value_(value) {}
  Value value_;
  friend class utils::Impl;
};

class PathAttributes
  : public utils::CPtrView<PathAttributes, const parsebgp_bgp_update_path_attrs*> {
public:
  class Flag : public utils::FlagClass<Flag> {
  public:
    enum Mask {
      OPTIONAL = 0x80,
      TRANSITIVE = 0x40,
      PARTIAL = 0x20,
      EXTENDED = 0x10,
    };

    uint8_t value() const { return value_; }

    bool is_optional() const { return value_ & OPTIONAL; }
    bool is_transitive() const { return value_ & TRANSITIVE; }
    bool is_partial() const { return value_ & PARTIAL; }
    bool is_extended() const { return value_ & EXTENDED; }

  private:
    uint8_t value_;
    friend class utils::Impl;
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

    Value value() const { return value_; }
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
    Type(Value value) : value_(value) {}
    Value value_;
    friend class utils::Impl;
  };

private:
  class Generic : public utils::CPtrView<Generic, const parsebgp_bgp_update_path_attr*> {
  public:
    Flag flag() const;
    Type type() const;

  private:
    using BaseView::BaseView;
    friend class utils::Impl;
  };

#define CLASS_FOR_GENERIC_PATH_ATTRIBUTE(ClassName_, ValueType_)                                   \
  class ClassName_ : public Generic {                                                              \
  public:                                                                                          \
    ValueType_ value() const;                                                                      \
    operator ValueType_() const;                                                                   \
                                                                                                   \
  private:                                                                                         \
    using Generic::Generic;                                                                        \
    friend class utils::Impl;                                                                      \
  }

public:
  CLASS_FOR_GENERIC_PATH_ATTRIBUTE(Origin, uint8_t);
  CLASS_FOR_GENERIC_PATH_ATTRIBUTE(NextHop, utils::ipv4_view);
  CLASS_FOR_GENERIC_PATH_ATTRIBUTE(Med, uint32_t);
  CLASS_FOR_GENERIC_PATH_ATTRIBUTE(LocalPref, uint32_t);

  class Aggregator : public Generic {
  public:
    uint32_t asn() const;
    utils::ipv4_view addr() const;

  private:
    using Generic::Generic;
    friend class utils::Impl;
  };

  utils::optional<Origin> origin() const;
  utils::optional<NextHop> next_hop() const;
  utils::optional<Med> med() const;
  utils::optional<LocalPref> local_pref() const;
  utils::optional<Aggregator> aggregator() const;

  // TODO: Rest of path attributes

private:
  using BaseView::BaseView;
  friend class utils::Impl;
};

} // namespace bgp
} // namespace parsebgp
