#include <type_traits>

#include <parsebgp/bgp/common.hpp>
#include <parsebgp_bgp_common.h>

namespace parsebgp {
namespace bgp {

//==============================================================================
// bgp::AfiType
//==============================================================================

static_assert(AfiType::IPV4 == int(PARSEBGP_BGP_AFI_IPV4));
static_assert(AfiType::IPV6 == int(PARSEBGP_BGP_AFI_IPV6));

//==============================================================================
// bgp::SafiType
//==============================================================================

static_assert(SafiType::UNICAST == int(PARSEBGP_BGP_SAFI_UNICAST));
static_assert(SafiType::MULTICAST == int(PARSEBGP_BGP_SAFI_MULTICAST));
static_assert(SafiType::MULTICAST == int(PARSEBGP_BGP_SAFI_MULTICAST));

//==============================================================================
// bgp::PrefixType
//==============================================================================

static_assert(PrefixType::UNICAST_IPV4 == int(PARSEBGP_BGP_PREFIX_UNICAST_IPV4));
static_assert(PrefixType::UNICAST_IPV6 == int(PARSEBGP_BGP_PREFIX_UNICAST_IPV6));
static_assert(PrefixType::MULTICAST_IPV4 == int(PARSEBGP_BGP_PREFIX_MULTICAST_IPV4));
static_assert(PrefixType::MULTICAST_IPV6 == int(PARSEBGP_BGP_PREFIX_MULTICAST_IPV6));

//==============================================================================
// bgp::Prefix
//==============================================================================

PrefixType Prefix::type() const {
  return PrefixType::Value(cptr()->type);
}

AfiType Prefix::afi_type() const {
  return AfiType::Value(cptr()->afi);
}

SafiType Prefix::safi_type() const {
  return SafiType::Value(cptr()->safi);
}

uint8_t Prefix::len() const {
  return cptr()->len;
}

utils::ipv6_view Prefix::addr() const {
  return cptr()->addr;
}

//==============================================================================
// bgp::Asn
//==============================================================================

static_assert(std::is_standard_layout_v<Asn> && sizeof(Asn) == 4,
              "Asn class should have standard layout compatibility with 32-bit unsigned integer.");

} // namespace bgp
} // namespace parsebgp
