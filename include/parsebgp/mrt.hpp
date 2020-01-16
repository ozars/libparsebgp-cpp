#pragma once

#include <cassert>
#include <cstdint>

#include <parsebgp/bgp/common.hpp>
#include <parsebgp/bgp/update.hpp>
#include <parsebgp/utils.hpp>

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

class Message;
class AsnType;

namespace table_dump_v2 {

class PeerEntry;
class PeerIndex;
class RibEntry;
class Rib;
class Message;

} // namespace table_dump_v2
} // namespace mrt
} // namespace parsebgp

namespace parsebgp {
namespace mrt {

using bgp::Asn;
using bgp::AfiType;

class Message : public utils::CPtrView<Message, parsebgp_mrt_msg*> {
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

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    Type(Value value) : value_(value) {}

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    operator Value() { return value_; }

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
      }
      return false;
    }
    bool is_bgp() const { return value_ == BGP; }
    bool is_ospf_v2() const { return value_ == OSPF_V2; }
    bool is_table_dump() const { return value_ == TABLE_DUMP; }
    bool is_table_dump_v2() const { return value_ == TABLE_DUMP_V2; }
    bool is_bgp4mp() const { return value_ == BGP4MP || value_ == BGP4MP_ET; }
    bool is_isis() const { return value_ == ISIS || value_ == ISIS_ET; }
    bool is_ospf_v3() const { return value_ == OSPF_V3 || value_ == OSPF_V3_ET; }

  private:
    Value value_;
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  Message(CPtr cptr) : BaseView(cptr) {}

  void dump(int depth = 0) const;

  Type type() const;
  uint16_t subtype() const;
  uint32_t length() const;

  table_dump_v2::Message to_table_dump_v2() const;
};

class AsnType : public utils::EnumClass<AsnType> {
public:
  enum Value {
    ASN_2_BYTE = 0,
    ASN_4_BYTE = 1,
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  AsnType(Value value) : value_(value) {}

  Value value() const { return value_; }
  bool is_valid() const { return value_ == ASN_2_BYTE || value_ == ASN_4_BYTE; }
  bool is_asn_2_byte() const { return value_ == ASN_2_BYTE; }
  bool is_asn_4_byte() const { return value_ == ASN_4_BYTE; }

private:
  Value value_;
};

namespace table_dump_v2 {

class PeerEntry : public utils::CPtrView<PeerEntry, parsebgp_mrt_table_dump_v2_peer_entry*> {
public:
  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  PeerEntry(CPtr cptr) : BaseView(cptr) {}

  AsnType asn_type() const;
  AfiType ip_afi() const;
  utils::ipv4_view bgp_id() const;
  utils::bytes_view raw_ip() const;
  utils::ip_view ip() const;
  utils::ipv4_view ipv4() const;
  utils::ipv6_view ipv6() const;
  uint32_t asn() const;
};

class PeerIndex
  : public utils::CPtrView<PeerIndex, parsebgp_mrt_table_dump_v2_peer_index*>
  , utils::CPtrRange<PeerIndex, PeerEntry> {
public:
  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  PeerIndex(CPtr cptr) : BaseView(cptr) {}

  utils::ipv4_view collector_bgp_id() const;
  utils::string_view view_name() const;

private:
  friend BaseRange;
  ElementCPtr range_data() const;
  std::size_t range_size() const;
  static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n);
  static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n);
  static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs);
};

class RibEntry : public utils::CPtrView<RibEntry, parsebgp_mrt_table_dump_v2_rib_entry*> {
public:
  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  RibEntry(CPtr cptr) : BaseView(cptr) {}

  uint16_t peer_index() const;
  uint32_t originated_time() const;
  bgp::PathAttributes path_attributes() const;
};

class Rib
  : public utils::CPtrView<Rib, parsebgp_mrt_table_dump_v2_afi_safi_rib*>
  , public utils::CPtrRange<Rib, RibEntry> {
public:
  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  Rib(CPtr cptr) : BaseView(cptr) {}

  uint32_t sequence_no() const;
  uint8_t prefix_len() const;
  utils::ipv6_view prefix() const;

private:
  friend BaseRange;
  ElementCPtr range_data() const;
  std::size_t range_size() const;
  static ElementCPtr range_add(const ElementCPtr ptr, std::ptrdiff_t n);
  static ElementCPtr range_subtract(const ElementCPtr ptr, std::ptrdiff_t n);
  static std::ptrdiff_t range_difference(const ElementCPtr lhs, const ElementCPtr rhs);
};

class Message : public utils::CPtrView<Message, parsebgp_mrt_msg*> {
public:
  class Subtype : public utils::EnumClass<Subtype> {
  public:
    enum Value : uint16_t {
      PEER_INDEX_TABLE = 1,
      RIB_IPV4_UNICAST = 2,
      RIB_IPV4_MULTICAST = 3,
      RIB_IPV6_UNICAST = 4,
      RIB_IPV6_MULTICAST = 5,
      RIB_GENERIC = 6,
    };

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    Subtype(Value value) : value_(value) {}

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    operator Value() { return value_; }

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
      }
      return false;
    }

    bool is_peer_index_table() const { return value_ == PEER_INDEX_TABLE; }
    bool is_rib_ipv4_unicast() const { return value_ == RIB_IPV4_UNICAST; }
    bool is_rib_ipv4_multicast() const { return value_ == RIB_IPV4_MULTICAST; }
    bool is_rib_ipv6_unicast() const { return value_ == RIB_IPV6_UNICAST; }
    bool is_rib_ipv6_multicast() const { return value_ == RIB_IPV6_MULTICAST; }
    bool is_rib_generic() const { return value_ == RIB_GENERIC; }

    bool is_rib_ip() const { return value_ >= RIB_IPV4_UNICAST && value_ <= RIB_IPV6_MULTICAST; }
    bool is_rib() const { return is_rib_ip() || is_rib_generic(); }
    bool is_rib_ip_unicast() const { return is_rib_ipv4_unicast() || is_rib_ipv6_unicast(); }
    bool is_rib_ip_multicast() const { return is_rib_ipv4_multicast() || is_rib_ipv6_multicast(); }

  private:
    Value value_;
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  Message(CPtr cptr) : BaseView(cptr) {}

  void dump(int depth = 0) const;

  mrt::Message::Type type() const;
  Subtype subtype() const;
  uint32_t length() const;

  PeerIndex to_peer_index() const;
  Rib to_rib() const;
};

} // namespace table_dump_v2
} // namespace mrt
} // namespace parsebgp
