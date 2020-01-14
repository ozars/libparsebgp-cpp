#include <parsebgp/bgp/update.hpp>
#include <parsebgp_bgp_update.h>

namespace parsebgp {
namespace bgp {

static bool has_attr_type(parsebgp_bgp_update_path_attrs_t* cptr, PathAttributes::Type type) {
  return cptr->attrs[type.value()].type == type.value();
}

//==============================================================================
// bgp::PathAttributes
//==============================================================================

static_assert(int(PathAttributes::Type::ORIGIN) == PARSEBGP_BGP_PATH_ATTR_TYPE_ORIGIN);
static_assert(int(PathAttributes::Type::AS_PATH) == PARSEBGP_BGP_PATH_ATTR_TYPE_AS_PATH);
static_assert(int(PathAttributes::Type::NEXT_HOP) == PARSEBGP_BGP_PATH_ATTR_TYPE_NEXT_HOP);
static_assert(int(PathAttributes::Type::MED) == PARSEBGP_BGP_PATH_ATTR_TYPE_MED);
static_assert(int(PathAttributes::Type::LOCAL_PREF) == PARSEBGP_BGP_PATH_ATTR_TYPE_LOCAL_PREF);
static_assert(int(PathAttributes::Type::ATOMIC_AGGREGATE) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_ATOMIC_AGGREGATE);
static_assert(int(PathAttributes::Type::AGGREGATOR) == PARSEBGP_BGP_PATH_ATTR_TYPE_AGGREGATOR);
static_assert(int(PathAttributes::Type::COMMUNITIES) == PARSEBGP_BGP_PATH_ATTR_TYPE_COMMUNITIES);
static_assert(int(PathAttributes::Type::ORIGINATOR_ID) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_ORIGINATOR_ID);
static_assert(int(PathAttributes::Type::CLUSTER_LIST) == PARSEBGP_BGP_PATH_ATTR_TYPE_CLUSTER_LIST);
static_assert(int(PathAttributes::Type::MP_REACH_NLRI) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_MP_REACH_NLRI);
static_assert(int(PathAttributes::Type::MP_UNREACH_NLRI) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_MP_UNREACH_NLRI);
static_assert(int(PathAttributes::Type::EXT_COMMUNITIES) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_EXT_COMMUNITIES);
static_assert(int(PathAttributes::Type::AS4_PATH) == PARSEBGP_BGP_PATH_ATTR_TYPE_AS4_PATH);
static_assert(int(PathAttributes::Type::AS4_AGGREGATOR) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_AS4_AGGREGATOR);
static_assert(int(PathAttributes::Type::AS_PATHLIMIT) == PARSEBGP_BGP_PATH_ATTR_TYPE_AS_PATHLIMIT);
static_assert(int(PathAttributes::Type::IPV6_EXT_COMMUNITIES) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_IPV6_EXT_COMMUNITIES);
static_assert(int(PathAttributes::Type::BGP_LS) == PARSEBGP_BGP_PATH_ATTR_TYPE_BGP_LS);
static_assert(int(PathAttributes::Type::LARGE_COMMUNITIES) ==
              PARSEBGP_BGP_PATH_ATTR_TYPE_LARGE_COMMUNITIES);

bool PathAttributes::has_origin() const {
  return has_attr_type(cptr(), Type::ORIGIN);
}

auto PathAttributes::origin() const -> Origin {
  assert(has_origin());
  return &cptr()->attrs[Type::ORIGIN];
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

//==============================================================================
// bgp::PathAttributes::Base
//==============================================================================

auto PathAttributes::Base::flags() const -> Flags {
  return Flags(cptr()->flags);
}

auto PathAttributes::Base::type() const -> Type {
  return Type::Value(cptr()->type);
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

} // namespace bgp
} // namespace parsebgp
