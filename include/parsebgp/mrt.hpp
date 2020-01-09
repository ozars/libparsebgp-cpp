#pragma once

#include <cstdint>

#include "parsebgp/bgp/update.hpp"
#include "parsebgp/mrt_fwd.hpp"
#include "parsebgp/utils.hpp"

extern "C" {
struct parsebgp_mrt_table_dump;
struct parsebgp_mrt_table_dump_v2_peer_entry;
struct parsebgp_mrt_table_dump_v2_peer_index;
struct parsebgp_mrt_table_dump_v2_rib_entry;
struct parsebgp_mrt_table_dump_v2_afi_safi_rib;
struct parsebgp_mrt_table_dump_v2;
struct parsebgp_mrt_bgp4mp_state_change;
struct parsebgp_mrt_bgp;
struct parsebgp_mrt_bgp4mp;
struct parsebgp_mrt_msg;
}

namespace parsebgp {
namespace mrt {

class Message : public utils::CPtrView<Message, const parsebgp_mrt_msg*> {
public:
  class Type : public utils::EnumClass<Type> {
  public:
    enum Value : uint16_t {
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

    bool is_valid() const {
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
    bool is_ospf_v3() const { return value_ == OSPF_V3 || value_ == OSPF_V3_ET; }

  private:
    Type(Value value) : value_(value) {}
    Value value_;
    friend class utils::Impl;
  };

  Type type() const;
  uint16_t subtype() const;
  uint32_t length() const;

  utils::optional<table_dump_v2::Message> to_table_dump_v2() const;

private:
  using BaseView::BaseView;
  friend class utils::Impl;
};

class AsnType : public utils::EnumClass<AsnType> {
public:
  enum Value {
    ASN_2_BYTE = 0,
    ASN_4_BYTE = 1,
  };

  Value value() const { return value_; }
  bool is_valid() const { return value_ == ASN_2_BYTE || value_ == ASN_4_BYTE; }
  bool is_asn_2_byte() const { return value_ == ASN_2_BYTE; }
  bool is_asn_4_byte() const { return value_ == ASN_4_BYTE; }

private:
  AsnType(Value value) : value_(value) {}
  Value value_;
  friend class utils::Impl;
};

namespace table_dump_v2 {

class PeerEntry
  : public utils::CPtrView<PeerEntry, const parsebgp_mrt_table_dump_v2_peer_entry*>
  , utils::IteratorClass<PeerEntry> {
public:
  AsnType asn_type() const;
  bgp::AfiType ip_afi() const;
  utils::ipv4_view bgp_id() const;
  utils::ipv6_view ip() const;
  uint32_t asn() const;

  PeerEntry& operator++();
  const PeerEntry& operator*() const { return *this; }
  bool operator==(const PeerEntry& rhs) const { return cptr() == rhs.cptr(); }
  bool operator!=(const PeerEntry& rhs) const { return cptr() != rhs.cptr(); }

private:
  using BaseView::BaseView;
  friend class utils::Impl;
};

class PeerIndex
  : public utils::CPtrView<PeerIndex, const parsebgp_mrt_table_dump_v2_peer_index*>
  , utils::RangeClass<PeerIndex> {
public:
  utils::ipv4_view collector_bgp_id() const;
  utils::string_view view_name() const;

  PeerEntry begin() const;
  PeerEntry end() const;
  uint16_t size() const;

private:
  using BaseView::BaseView;
  friend class utils::Impl;
};

class RibEntry
  : public utils::CPtrView<RibEntry, const parsebgp_mrt_table_dump_v2_rib_entry*>
  , utils::IteratorClass<RibEntry> {
public:
  uint16_t peer_index() const;
  uint32_t originated_time() const;

  bgp::PathAttributes path_attributes() const;
  // bgp::PathAttributes path_attrs() const { return path_attributes(); }

  RibEntry& operator++();
  const RibEntry& operator*() const { return *this; }
  bool operator==(const RibEntry& rhs) const { return cptr() == rhs.cptr(); }
  bool operator!=(const RibEntry& rhs) const { return cptr() != rhs.cptr(); }

private:
  using BaseView::BaseView;
  friend class utils::Impl;
};

class Rib
  : public utils::CPtrView<Rib, const parsebgp_mrt_table_dump_v2_afi_safi_rib*>
  , utils::RangeClass<Rib> {
public:
  uint32_t sequence_no() const;
  // uint32_t sequence() const { return sequence_no; }
  uint8_t prefix_len() const;
  utils::ipv6_view prefix() const;

  RibEntry begin() const;
  RibEntry end() const;
  uint16_t size() const;

private:
  using BaseView::BaseView;
  friend class utils::Impl;
};

class Message : public utils::CPtrView<Message, const parsebgp_mrt_msg*> {
public:
  class Subtype : public utils::EnumClass<Subtype> {
  public:
    enum Value {
      PEER_INDEX_TABLE = 1,
      RIB_IPV4_UNICAST = 2,
      RIB_IPV4_MULTICAST = 3,
      RIB_IPV6_UNICAST = 4,
      RIB_IPV6_MULTICAST = 5,
      RIB_GENERIC = 6,
    };

    Value value() const { return value_; }
    bool is_valid() const {
      switch (value_) {

        case PEER_INDEX_TABLE:
        case RIB_IPV4_UNICAST:
        case RIB_IPV4_MULTICAST:
        case RIB_IPV6_UNICAST:
        case RIB_IPV6_MULTICAST:
        case RIB_GENERIC:
          return true;
        default:
          return false;
      }
    }

    bool is_peer_index_table() const { return value_ == PEER_INDEX_TABLE; }
    bool is_rib_ipv4_unicast() const { return value_ == RIB_IPV4_UNICAST; }
    bool is_rib_ipv4_multicast() const { return value_ == RIB_IPV4_MULTICAST; }
    bool is_rib_ipv6_unicast() const { return value_ == RIB_IPV6_UNICAST; }
    bool is_rib_ipv6_multicast() const { return value_ == RIB_IPV6_MULTICAST; }
    bool is_rib_generic() const { return value_ == RIB_GENERIC; }

    bool is_rib_ip() const { return value_ >= RIB_IPV4_UNICAST && value_ <= RIB_IPV6_MULTICAST; }

  private:
    Subtype(Value value) : value_(value) {}
    Value value_;
    friend class utils::Impl;
  };

  mrt::Message::Type type() const;
  Subtype subtype() const;
  uint32_t length() const;

  utils::optional<PeerIndex> to_peer_index() const;
  utils::optional<Rib> to_rib() const;

private:
  using BaseView::BaseView;
  friend class utils::Impl;
};

} // namespace table_dump_v2
} // namespace mrt
} // namespace parsebgp
