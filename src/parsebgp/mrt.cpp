#include <parsebgp/mrt.hpp>
#include <parsebgp_mrt.h>

namespace parsebgp {
namespace mrt {

static_assert(Message::Type::BGP == int(PARSEBGP_MRT_TYPE_BGP));
static_assert(Message::Type::OSPF_V2 == int(PARSEBGP_MRT_TYPE_OSPF_V2));
static_assert(Message::Type::TABLE_DUMP == int(PARSEBGP_MRT_TYPE_TABLE_DUMP));
static_assert(Message::Type::TABLE_DUMP_V2 == int(PARSEBGP_MRT_TYPE_TABLE_DUMP_V2));
static_assert(Message::Type::BGP4MP == int(PARSEBGP_MRT_TYPE_BGP4MP));
static_assert(Message::Type::BGP4MP_ET == int(PARSEBGP_MRT_TYPE_BGP4MP_ET));
static_assert(Message::Type::ISIS == int(PARSEBGP_MRT_TYPE_ISIS));
static_assert(Message::Type::ISIS_ET == int(PARSEBGP_MRT_TYPE_ISIS_ET));
static_assert(Message::Type::OSPF_V3 == int(PARSEBGP_MRT_TYPE_OSPF_V3));
static_assert(Message::Type::OSPF_V3_ET == int(PARSEBGP_MRT_TYPE_OSPF_V3_ET));

static_assert(AsnType::ASN_2_BYTE == int(PARSEBGP_MRT_ASN_2_BYTE));
static_assert(AsnType::ASN_4_BYTE == int(PARSEBGP_MRT_ASN_4_BYTE));

//==============================================================================
// mrt::Message
//==============================================================================

auto Message::type() const -> Type {
  return Type::Value(cptr()->type);
}

uint16_t Message::subtype() const {
  return cptr()->subtype;
}

uint32_t Message::length() const {
  return cptr()->len;
}

const table_dump_v2::Message Message::to_table_dump_v2() const {
  return table_dump_v2::Message(cptr());
}

//==============================================================================
// mrt::table_dump_v2
//==============================================================================

namespace table_dump_v2 {

static_assert(Message::Subtype::PEER_INDEX_TABLE ==
              int(PARSEBGP_MRT_TABLE_DUMP_V2_PEER_INDEX_TABLE));
static_assert(Message::Subtype::RIB_IPV4_UNICAST ==
              int(PARSEBGP_MRT_TABLE_DUMP_V2_RIB_IPV4_UNICAST));
static_assert(Message::Subtype::RIB_IPV4_MULTICAST ==
              int(PARSEBGP_MRT_TABLE_DUMP_V2_RIB_IPV4_MULTICAST));
static_assert(Message::Subtype::RIB_IPV6_UNICAST ==
              int(PARSEBGP_MRT_TABLE_DUMP_V2_RIB_IPV6_UNICAST));
static_assert(Message::Subtype::RIB_IPV6_MULTICAST ==
              int(PARSEBGP_MRT_TABLE_DUMP_V2_RIB_IPV6_MULTICAST));
static_assert(Message::Subtype::RIB_GENERIC == int(PARSEBGP_MRT_TABLE_DUMP_V2_RIB_GENERIC));

//==============================================================================
// mrt::table_dump_v2::PeerEntry
//==============================================================================

AsnType PeerEntry::asn_type() const {
  return AsnType::Value(cptr()->asn_type);
}

bgp::AfiType PeerEntry::ip_afi() const {
  return bgp::AfiType::Value(cptr()->ip_afi);
}

const utils::ipv4_span PeerEntry::bgp_id() const {
  return cptr()->bgp_id;
}

const utils::ipv6_span PeerEntry::ip() const {
  return cptr()->ip;
}

uint32_t PeerEntry::asn() const {
  return cptr()->asn;
}

//==============================================================================
// mrt::table_dump_v2::PeerIndex
//==============================================================================

const utils::ipv4_span PeerIndex::collector_bgp_id() const {
  return cptr()->collector_bgp_id;
}

utils::string_view PeerIndex::view_name() const {
  return { cptr()->view_name, cptr()->view_name_len };
}

auto PeerIndex::range_data() const -> ElementCPtr {
  return cptr()->peer_entries;
}

std::size_t PeerIndex::range_size() const {
  return cptr()->peer_count;
}

auto PeerIndex::range_add(const ElementCPtr ptr, std::ptrdiff_t n) -> ElementCPtr {
  return ptr + n;
}

auto PeerIndex::range_subtract(const ElementCPtr ptr, std::ptrdiff_t n) -> ElementCPtr {
  return ptr - n;
}

std::ptrdiff_t PeerIndex::range_difference(const ElementCPtr lhs, const ElementCPtr rhs) {
  return lhs - rhs;
}

//==============================================================================
// mrt::table_dump_v2::RibEntry
//==============================================================================

uint16_t RibEntry::peer_index() const {
  return cptr()->peer_index;
}

uint32_t RibEntry::originated_time() const {
  return cptr()->originated_time;
}

const bgp::PathAttributes RibEntry::path_attributes() const {
  return bgp::PathAttributes(&cptr()->path_attrs);
}

//==============================================================================
// mrt::table_dump_v2::Rib
//==============================================================================

uint32_t Rib::sequence_no() const {
  return cptr()->sequence;
}

uint8_t Rib::prefix_len() const {
  return cptr()->prefix_len;
}

const utils::ipv6_span Rib::prefix() const {
  return cptr()->prefix;
}

auto Rib::range_data() const -> ElementCPtr {
  return cptr()->entries;
}

std::size_t Rib::range_size() const {
  return cptr()->entry_count;
}

auto Rib::range_add(const ElementCPtr ptr, std::ptrdiff_t n) -> ElementCPtr {
  return ptr + n;
}

auto Rib::range_subtract(const ElementCPtr ptr, std::ptrdiff_t n) -> ElementCPtr {
  return ptr - n;
}

std::ptrdiff_t Rib::range_difference(const ElementCPtr lhs, const ElementCPtr rhs) {
  return lhs - rhs;
}

//==============================================================================
// mrt::table_dump_v2::Message
//==============================================================================

mrt::Message::Type Message::type() const {
  return mrt::Message::Type::Value(cptr()->type);
}

auto Message::subtype() const -> Subtype {
  return Subtype::Value(cptr()->subtype);
}

uint32_t Message::length() const {
  return uint32_t(cptr()->len);
}

const PeerIndex Message::to_peer_index() const {
  return PeerIndex(&cptr()->types.table_dump_v2->peer_index);
}

const Rib Message::to_rib() const {
  return Rib(&cptr()->types.table_dump_v2->afi_safi_rib);
}

} // namespace table_dump_v2
} // namespace mrt
} // namespace parsebgp
