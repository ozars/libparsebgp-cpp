#pragma once

#include <cstdint>

#include "parsebgp/utils.hpp"

namespace parsebgp {
namespace bgp {

class AfiType : public utils::EnumClass<AfiType>
{
public:
  enum Value
  {
    IPV4 = 1,
    IPV6 = 2,
  };

  Value value() const { return value_; }
  bool is_valid() const { return value_ == IPV4 || value_ == IPV6; }
  bool is_ipv4() const { return value_ == IPV4; }
  bool is_ipv6() const { return value_ == IPV6; }

private:
  Value value_;
  friend class utils::Impl;
};

class PathAttributes : public utils::OpaqueView<PathAttributes>
{
public:
  class Flag : public utils::FlagClass<Flag>
  {
  public:
    uint8_t value() const { return value_; }

    bool is_optional() const;
    bool is_transitive() const;
    bool is_partial() const;
    bool is_extended() const;

  private:
    uint8_t value_;
    friend class utils::Impl;
  };

  class Type : public utils::EnumClass<Type>
  {
  public:
    enum Value
    {
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

    Value value() const;
    bool is_origin() const;
    bool is_as_path() const;
    bool is_next_hop() const;
    bool is_med() const;
    bool is_local_pref() const;
    bool is_atomic_aggregate() const;
    bool is_aggregator() const;
    bool is_communities() const;
    bool is_originator_id() const;
    bool is_cluster_list() const;
    bool is_mp_reach_nlri() const;
    bool is_mp_unreach_nlri() const;
    bool is_ext_communities() const;
    bool is_as4_path() const;
    bool is_as4_aggregator() const;
    bool is_as_pathlimit() const;
    bool is_ipv6_ext_communities() const;
    bool is_bgp_ls() const;
    bool is_large_communities() const;

  private:
    Value value_;
    friend class utils::Impl;
  };

private:
  class Generic : public utils::OpaqueView<Generic>
  {
  public:
    Flag flag() const;
    Type type() const;

  private:
    using OpaqueView<Generic>::OpaqueView;
    friend class utils::Impl;
  };

  template<typename Value>
  class WrappedValue : public Generic
  {
  public:
    Value value() const;
    operator Value() const;

  private:
    using Generic::Generic;
    friend class utils::Impl;
  };

public:
  using Origin = WrappedValue<uint8_t>;
  using NextHop = WrappedValue<utils::array_view<uint8_t, 4>>;
  using Med = WrappedValue<uint32_t>;
  using LocalPref = WrappedValue<uint32_t>;

  class Aggregator : public Generic
  {
  public:
    uint32_t asn() const;
    utils::array_view<uint8_t, 4> addr() const;

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
  using OpaqueView<PathAttributes>::OpaqueView;
  friend class utils::Impl;
};

} // namespace bgp
} // namespace parsebgp
