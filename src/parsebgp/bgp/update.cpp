#include <parsebgp/bgp/update.hpp>
#include <parsebgp_bgp_update.h>

namespace parsebgp {
namespace bgp {

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

const utils::ipv4_span PathAttributes::Aggregator::addr() const {
  return cptr()->data.aggregator.addr;
}

} // namespace bgp
} // namespace parsebgp
