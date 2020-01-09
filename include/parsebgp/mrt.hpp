#pragma once

#include <cstdint>

#include "parsebgp/bgp/update.hpp"
#include "parsebgp/mrt_fwd.hpp"
#include "parsebgp/utils.hpp"

namespace parsebgp {
namespace mrt {

class Message : public utils::OpaqueView<Message>
{
public:
  class Type : public utils::EnumClass<Type>
  {
  public:
    enum Value : uint16_t
    {
      BGP = 5,
      OSPF_V2 = 11,
      TABLE_DUMP = 12,
      TABLE_DUMP_V2 = 13,
      BGP4MP = 16,
      BGP4MP_ET = 17,
      ISIS = 32,
      ISIS_ET = 33,
      OSPF_V3 = 48,
      OSPF_V3_ET = 49,
    };

    Value value() const { return value_; }

    bool is_valid() const
    {
      switch (value_) {
        case BGP:
        case OSPF_V2:
        case TABLE_DUMP:
        case TABLE_DUMP_V2:
        case BGP4MP:
        case BGP4MP_ET:
        case ISIS:
        case ISIS_ET:
        case OSPF_V3:
        case OSPF_V3_ET:
          return true;
        default:
          return false;
      }
    }

    bool is_bgp() const { return value_ == BGP; }
    bool is_ospf_v2() const { return value_ == OSPF_V2; }
    bool is_table_dump() const { return value_ == TABLE_DUMP; }
    bool is_table_dump_v2() const { return value_ == TABLE_DUMP_V2; }
    bool is_bgp4mp() const { return value_ == BGP4MP || value_ == BGP4MP_ET; }
    bool is_isis() const { return value_ == ISIS || value_ == ISIS_ET; }
    bool is_ospf_v3() const
    {
      return value_ == OSPF_V3 || value_ == OSPF_V3_ET;
    }

  private:
    Value value_;
    friend class utils::Impl;
  };

  Type type() const;
  uint16_t subtype() const;
  uint32_t length() const;

  utils::optional<table_dump_v2::Message> to_table_dump_v2() const;

private:
  using OpaqueView<Message>::OpaqueView;
  friend class utils::Impl;
};

class AsnType : public utils::EnumClass<AsnType>
{
public:
  enum Value
  {
    ASN_2_BYTE = 0,
    ASN_4_BYTE = 1,
  };

  Value value() const;
  bool is_valid() const;
  bool is_asn_2_byte() const;
  bool is_asn_4_byte() const;

private:
  Value value_;
  friend class utils::Impl;
};

namespace table_dump_v2 {

class PeerEntry
  : public utils::OpaqueView<PeerEntry>
  , utils::IteratorClass<PeerEntry>
{
public:
  AsnType asn_type() const;
  bgp::AfiType ip_afi() const;
  utils::array_view<uint8_t, 4> bgp_id() const;
  utils::array_view<uint8_t, 4> ipv4() const;
  utils::array_view<uint8_t, 16> ipv6() const;
  uint32_t asn() const;

  PeerEntry& operator++();
  PeerEntry& operator*() const;
  bool operator==(const PeerEntry&) const;
  bool operator!=(const PeerEntry&) const;

private:
  using OpaqueView<PeerEntry>::OpaqueView;
  friend class utils::Impl;
};

class PeerIndex
  : public utils::OpaqueView<PeerIndex>
  , utils::RangeClass<PeerIndex>
{
public:
  utils::array_view<uint8_t, 4> collector_bgp_id() const;
  utils::string_view view_name() const;

  PeerEntry begin() const;
  PeerEntry end() const;
  size_t size() const;

private:
  using OpaqueView<PeerIndex>::OpaqueView;
  friend class utils::Impl;
};

class RibEntry
  : public utils::OpaqueView<RibEntry>
  , utils::IteratorClass<RibEntry>
{
public:
  uint16_t peer_index() const;
  uint32_t originated_time() const;

  PathAttributes attributes() const;

  RibEntry& operator++();
  const RibEntry& operator*() const;
  bool operator==(const RibEntry&) const;
  bool operator!=(const RibEntry&) const;

private:
  using OpaqueView<RibEntry>::OpaqueView;
  friend class utils::Impl;
};

class Rib
  : public utils::OpaqueView<Rib>
  , utils::RangeClass<Rib>
{
public:
  uint32_t sequence_no() const;
  uint8_t prefix_len() const;
  utils::array_view<uint8_t, 16> prefix() const;

  RibEntry begin() const;
  RibEntry end() const;
  size_t size() const;

private:
  using OpaqueView<Rib>::OpaqueView;
  friend class utils::Impl;
};

class Message : public utils::OpaqueView<Message>
{
public:
  class Subtype : public utils::EnumClass<Subtype>
  {
  public:
    enum Value
    {
      PEER_INDEX_TABLE = 1,
      RIB_IPV4_UNICAST = 2,
      RIB_IPV4_MULTICAST = 3,
      RIB_IPV6_UNICAST = 4,
      RIB_IPV6_MULTICAST = 5,
      RIB_GENERIC = 6,
    };

    Value value() const;
    bool is_valid() const;
    bool is_peer_index_table() const;
    bool is_rib_ipv4_unicast() const;
    bool is_rib_ipv4_multicast() const;
    bool is_rib_ipv6_unicast() const;
    bool is_rib_ipv6_multicast() const;
    bool is_rib_generic() const;

  private:
    Value value_;
    friend class utils::Impl;
  };

  mrt::Message::Type type() const;
  Subtype subtype() const;
  uint32_t length() const;

  PeerIndex to_peer_index() const;
  Rib to_rib() const;

private:
  using OpaqueView<Message>::OpaqueView;
  friend class utils::Impl;
};

} // namespace table_dump_v2
} // namespace mrt
} // namespace parsebgp
