#include <parsebgp/bgp/update.hpp>
#include <parsebgp_bgp_update.h>

namespace parsebgp {
namespace bgp {

static bool has_attr_type(parsebgp_bgp_update_path_attrs_t* cptr,
                          PathAttributes::Type::Value type) {
  return cptr->attrs[type].type == type;
}

//==============================================================================
// bgp::PathAttributes
//==============================================================================

bool PathAttributes::has_origin() const {
  return has_attr_type(cptr(), Type::ORIGIN);
}

auto PathAttributes::origin() const -> Origin {
  assert(has_origin());
  return &cptr()->attrs[Type::ORIGIN];
}

bool PathAttributes::has_as_path() const {
  return has_attr_type(cptr(), Type::AS_PATH);
}

auto PathAttributes::as_path() const -> AsPath {
  assert(has_as_path());
  return &cptr()->attrs[Type::AS_PATH];
}

bool PathAttributes::has_next_hop() const {
  return has_attr_type(cptr(), Type::NEXT_HOP);
}

auto PathAttributes::next_hop() const -> NextHop {
  assert(has_next_hop());
  return &cptr()->attrs[Type::NEXT_HOP];
}

bool PathAttributes::has_med() const {
  return has_attr_type(cptr(), Type::MED);
}

auto PathAttributes::med() const -> Med {
  assert(has_med());
  return &cptr()->attrs[Type::MED];
}

bool PathAttributes::has_local_pref() const {
  return has_attr_type(cptr(), Type::LOCAL_PREF);
}

auto PathAttributes::local_pref() const -> LocalPref {
  assert(has_local_pref());
  return &cptr()->attrs[Type::LOCAL_PREF];
}

bool PathAttributes::has_aggregator() const {
  return has_attr_type(cptr(), Type::AGGREGATOR);
}

auto PathAttributes::aggregator() const -> Aggregator {
  assert(has_aggregator());
  return &cptr()->attrs[Type::AGGREGATOR];
}

bool PathAttributes::has_communities() const {
  return has_attr_type(cptr(), Type::COMMUNITIES);
}

auto PathAttributes::communities() const -> Communities {
  assert(has_communities());
  return &cptr()->attrs[Type::COMMUNITIES];
}

bool PathAttributes::has_cluster_list() const {
  return has_attr_type(cptr(), Type::CLUSTER_LIST);
}

auto PathAttributes::cluster_list() const -> ClusterList {
  assert(has_cluster_list());
  return &cptr()->attrs[Type::CLUSTER_LIST];
}

//==============================================================================
// bgp::PathAttributes::Type
//==============================================================================

static_assert(PathAttributes::Type::ORIGIN == int(PARSEBGP_BGP_PATH_ATTR_TYPE_ORIGIN));
static_assert(PathAttributes::Type::AS_PATH == int(PARSEBGP_BGP_PATH_ATTR_TYPE_AS_PATH));
static_assert(PathAttributes::Type::NEXT_HOP == int(PARSEBGP_BGP_PATH_ATTR_TYPE_NEXT_HOP));
static_assert(PathAttributes::Type::MED == int(PARSEBGP_BGP_PATH_ATTR_TYPE_MED));
static_assert(PathAttributes::Type::LOCAL_PREF == int(PARSEBGP_BGP_PATH_ATTR_TYPE_LOCAL_PREF));
static_assert(PathAttributes::Type::ATOMIC_AGGREGATE ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_ATOMIC_AGGREGATE));
static_assert(PathAttributes::Type::AGGREGATOR == int(PARSEBGP_BGP_PATH_ATTR_TYPE_AGGREGATOR));
static_assert(PathAttributes::Type::COMMUNITIES == int(PARSEBGP_BGP_PATH_ATTR_TYPE_COMMUNITIES));
static_assert(PathAttributes::Type::ORIGINATOR_ID ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_ORIGINATOR_ID));
static_assert(PathAttributes::Type::CLUSTER_LIST == int(PARSEBGP_BGP_PATH_ATTR_TYPE_CLUSTER_LIST));
static_assert(PathAttributes::Type::MP_REACH_NLRI ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_MP_REACH_NLRI));
static_assert(PathAttributes::Type::MP_UNREACH_NLRI ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_MP_UNREACH_NLRI));
static_assert(PathAttributes::Type::EXT_COMMUNITIES ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_EXT_COMMUNITIES));
static_assert(PathAttributes::Type::AS4_PATH == int(PARSEBGP_BGP_PATH_ATTR_TYPE_AS4_PATH));
static_assert(PathAttributes::Type::AS4_AGGREGATOR ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_AS4_AGGREGATOR));
static_assert(PathAttributes::Type::AS_PATHLIMIT == int(PARSEBGP_BGP_PATH_ATTR_TYPE_AS_PATHLIMIT));
static_assert(PathAttributes::Type::IPV6_EXT_COMMUNITIES ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_IPV6_EXT_COMMUNITIES));
static_assert(PathAttributes::Type::BGP_LS == int(PARSEBGP_BGP_PATH_ATTR_TYPE_BGP_LS));
static_assert(PathAttributes::Type::LARGE_COMMUNITIES ==
              int(PARSEBGP_BGP_PATH_ATTR_TYPE_LARGE_COMMUNITIES));

//==============================================================================
// bgp::PathAttributes::Base
//==============================================================================

auto PathAttributes::Base::flags() const -> Flags {
  return Flags(cptr()->flags);
}

auto PathAttributes::Base::type() const -> Type {
  return Type(Type::Value(cptr()->type));
}

//==============================================================================
// bgp::PathAttributes::{Origin, NextHop, Med, LocalPref}
//==============================================================================

#define PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(ClassName_, MemberName_)                               \
  auto PathAttributes::ClassName_::value() const->ValueType {                                      \
    return ValueType(cptr()->data.MemberName_);                                                    \
  }                                                                                                \
                                                                                                   \
  PathAttributes::ClassName_::operator ValueType() const { return value(); }

PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(Origin, origin)
PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(NextHop, next_hop)
PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(Med, med)
PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE(LocalPref, local_pref)

#undef PARSEBGP_CPP_GENERIC_PATH_ATTRIBUTE

//==============================================================================
// bgp::PathAttributes::AsPathSegment
//==============================================================================

auto PathAttributes::AsPathSegment::type() const -> Type {
  return Type::Value(cptr()->type);
}

auto PathAttributes::AsPathSegment::range_data() const -> ElementCPtr {
  return cptr()->asns;
}

std::size_t PathAttributes::AsPathSegment::range_size() const {
  return cptr()->asns_cnt;
}

//==============================================================================
// bgp::PathAttributes::AsPathSegment::Type
//==============================================================================

static_assert(PathAttributes::AsPathSegment::Type::AS_SET ==
              int(PARSEBGP_BGP_UPDATE_AS_PATH_SEG_AS_SET));
static_assert(PathAttributes::AsPathSegment::Type::AS_SEQ ==
              int(PARSEBGP_BGP_UPDATE_AS_PATH_SEG_AS_SEQ));
static_assert(PathAttributes::AsPathSegment::Type::CONFED_SEQ ==
              int(PARSEBGP_BGP_UPDATE_AS_PATH_SEG_CONFED_SEQ));
static_assert(PathAttributes::AsPathSegment::Type::CONFED_SET ==
              int(PARSEBGP_BGP_UPDATE_AS_PATH_SEG_CONFED_SET));

//==============================================================================
// bgp::PathAttributes::AsPath
//==============================================================================

uint8_t PathAttributes::AsPath::asns_count() const {
  return cptr()->data.as_path->asns_cnt;
}

bool PathAttributes::AsPath::asn_4_byte() const {
  return cptr()->data.as_path->asn_4_byte;
}

auto PathAttributes::AsPath::range_data() const -> ElementCPtr {
  return cptr()->data.as_path->segs;
}

std::size_t PathAttributes::AsPath::range_size() const {
  return cptr()->data.as_path->segs_cnt;
}

auto PathAttributes::AsPath::range_add(const ElementCPtr ptr, std::ptrdiff_t n) -> ElementCPtr {
  return ptr + n;
}

auto PathAttributes::AsPath::range_subtract(const ElementCPtr ptr, std::ptrdiff_t n)
  -> ElementCPtr {
  return ptr - n;
}

std::ptrdiff_t PathAttributes::AsPath::range_difference(const ElementCPtr lhs,
                                                        const ElementCPtr rhs) {
  return lhs - rhs;
}

//==============================================================================
// bgp::PathAttributes::Aggregator
//==============================================================================

uint32_t PathAttributes::Aggregator::asn() const {
  return cptr()->data.aggregator.asn;
}

utils::ipv4_view PathAttributes::Aggregator::addr() const {
  return cptr()->data.aggregator.addr;
}

//==============================================================================
// bgp::PathAttributes::Communities
//==============================================================================

auto PathAttributes::Communities::range_data() const -> ElementCPtr {
  return cptr()->data.communities->communities;
}

std::size_t PathAttributes::Communities::range_size() const {
  return cptr()->data.communities->communities_cnt;
}

//==============================================================================
// bgp::PathAttributes::ClusterList
//==============================================================================

auto PathAttributes::ClusterList::range_data() const -> ElementCPtr {
  return cptr()->data.cluster_list->cluster_ids;
}

std::size_t PathAttributes::ClusterList::range_size() const {
  return cptr()->data.cluster_list->cluster_ids_cnt;
}

} // namespace bgp
} // namespace parsebgp
