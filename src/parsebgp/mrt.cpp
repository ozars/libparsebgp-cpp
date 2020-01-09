#include "parsebgp/mrt.hpp"
#include "parsebgp/utils_impl.hpp"

#include "parsebgp_mrt.h"

namespace parsebgp {
namespace mrt {

using utils::Impl;

//==============================================================================
// mrt::Message
//==============================================================================

auto Message::type() const -> Type {
  return Impl{}.create<Type>(cptr()->type);
}

uint16_t Message::subtype() const {
  return cptr()->subtype;
}

uint32_t Message::length() const {
  return cptr()->len;
}

utils::optional<table_dump_v2::Message> Message::to_table_dump_v2() const {
  if (!type().is_table_dump_v2()) return utils::nullopt;
  return Impl{}.create<table_dump_v2::Message>(cptr());
}

namespace table_dump_v2 {

//==============================================================================
// mrt::table_dump_v2::PeerEntry
//==============================================================================

AsnType PeerEntry::asn_type() const {
  return Impl{}.create<AsnType>(cptr()->asn_type);
}

bgp::AfiType PeerEntry::ip_afi() const {
  return Impl{}.create<bgp::AfiType>(cptr()->ip_afi);
}

utils::array_view<uint8_t, 4> PeerEntry::bgp_id() const {
  return cptr()->bgp_id;
}

utils::array_view<uint8_t, 16> PeerEntry::ip() const {
  return cptr()->ip;
}

uint32_t PeerEntry::asn() const {
  return cptr()->asn;
}

PeerEntry& PeerEntry::operator++() {
  return *this = Impl{}.create<PeerEntry>(cptr() + 1);
}

//==============================================================================
// mrt::table_dump_v2::PeerIndex
//==============================================================================

utils::array_view<uint8_t, 4> PeerIndex::collector_bgp_id() const {
  return cptr()->collector_bgp_id;
}

utils::string_view PeerIndex::view_name() const {
  return { cptr()->view_name, cptr()->view_name_len };
}

PeerEntry PeerIndex::begin() const {
  return Impl{}.create<PeerEntry>(cptr()->peer_entries);
}

PeerEntry PeerIndex::end() const {
  return Impl{}.create<PeerEntry>(cptr()->peer_entries + cptr()->peer_count);
}

uint16_t PeerIndex::size() const {
  return cptr()->peer_count;
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

bgp::PathAttributes RibEntry::path_attributes() const {
  return Impl::create<bgp::PathAttributes>(&cptr()->path_attrs);
}

RibEntry& RibEntry::operator++() {
  return *this = Impl{}.create<RibEntry>(cptr() + 1);
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

utils::array_view<uint8_t, 16> Rib::prefix() const {
  return cptr()->prefix;
}

RibEntry Rib::begin() const {
  return Impl{}.create<RibEntry>(cptr()->entries);
}

RibEntry Rib::end() const {
  return Impl{}.create<RibEntry>(cptr()->entries + cptr()->entry_count);
}

uint16_t Rib::size() const {
  return cptr()->entry_count;
}

//==============================================================================
// mrt::table_dump_v2::Message
//==============================================================================

mrt::Message::Type Message::type() const {
  return Impl{}.create<mrt::Message::Type>(cptr()->type);
}

auto Message::subtype() const -> Subtype {
  return Impl{}.create<Subtype>(cptr()->subtype);
}

uint32_t Message::length() const {
  return Impl{}.create<uint32_t>(cptr()->len);
}

utils::optional<PeerIndex> Message::to_peer_index() const {
  if (!subtype().is_peer_index_table()) return utils::nullopt;
  return Impl{}.create<PeerIndex>(&cptr()->types.table_dump_v2->peer_index);
}

utils::optional<Rib> Message::to_rib() const {
  if (subtype().is_rib_ip()) return utils::nullopt;
  return Impl{}.create<Rib>(&cptr()->types.table_dump_v2->afi_safi_rib);
}

} // namespace table_dump_v2
} // namespace mrt
} // namespace parsebgp
