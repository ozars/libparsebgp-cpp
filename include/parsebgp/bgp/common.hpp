#pragma once

#include <cstdint>

#include <parsebgp/utils.hpp>

extern "C" struct parsebgp_bgp_prefix;

namespace parsebgp {
namespace bgp {

class AfiType : public utils::EnumClass<AfiType> {
public:
  enum Value : uint16_t {
    IPV4 = 1,
    IPV6 = 2,
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  AfiType(Value value) : value_(value) {}

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  operator Value() const { return value_; }

  Value value() const { return value_; }
  bool is_valid() const { return value_ == IPV4 || value_ == IPV6; }
  bool is_ipv4() const { return value_ == IPV4; }
  bool is_ipv6() const { return value_ == IPV6; }

private:
  Value value_;
};

class SafiType : public utils::EnumClass<SafiType> {
public:
  enum Value : uint8_t {
    UNICAST = 1,
    MULTICAST = 2,
    MPLS = 3,
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  SafiType(Value value) : value_(value) {}

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  operator Value() const { return value_; }

  Value value() const { return value_; }
  bool is_valid() const {
    switch (value_) {
      case UNICAST:
      case MULTICAST:
      case MPLS:
        return true;
    }
    return false;
  }
  bool is_unicast() const { return value_ == UNICAST; }
  bool is_multicast() const { return value_ == MULTICAST; }
  bool is_mpls() const { return value_ == MPLS; }

private:
  Value value_;
};

class PrefixType : public utils::EnumClass<PrefixType> {
public:
  enum Value : uint8_t {
    UNICAST_IPV4 = 1,
    UNICAST_IPV6 = 2,
    MULTICAST_IPV4 = 1,
    MULTICAST_IPV6 = 2,
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  PrefixType(Value value) : value_(value) {}

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  operator Value() const { return value_; }

  Value value() const { return value_; }
  bool is_valid() const {
    switch (value_) {
      case UNICAST_IPV4:
      case UNICAST_IPV6:
        // case MULTICAST_IPV4:
        // case MULTICAST_IPV6:
        static_assert(UNICAST_IPV4 == MULTICAST_IPV4);
        static_assert(UNICAST_IPV6 == MULTICAST_IPV6);
        return true;
    }
    return false;
  }
  bool is_unicast_ipv4() const { return value_ == UNICAST_IPV4; }
  bool is_multicast_ipv4() const { return value_ == MULTICAST_IPV4; }
  bool is_unicast_ipv6() const { return value_ == UNICAST_IPV6; }
  bool is_multicast_ipv6() const { return value_ == MULTICAST_IPV6; }

  bool is_unicast() const { return is_unicast_ipv4() || is_unicast_ipv6(); }
  bool is_multicast() const { return is_multicast_ipv4() || is_multicast_ipv6(); }
  bool is_ipv4() const { return is_unicast_ipv4() || is_multicast_ipv4(); }
  bool is_ipv6() const { return is_unicast_ipv6() || is_multicast_ipv6(); }

private:
  Value value_;
};

class Prefix : public utils::CPtrView<Prefix, parsebgp_bgp_prefix*> {
public:
  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  Prefix(CPtr cptr) : BaseView(cptr) {}

  PrefixType type() const;
  AfiType afi_type() const;
  SafiType safi_type() const;
  uint8_t len() const;
  utils::ipv6_view addr() const;
};

class Asn {
public:
  explicit Asn(uint32_t asn) : asn_(asn) {}

  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  Asn(uint32_t* asnp) : Asn(asnp ? *asnp : 0) {}

  // NOLINTNEXTLINE(google-explicit-constructor): Allow implicit conversion of AS number.
  operator uint32_t() const { return asn_; }

  bool is_16bit() const { return asn_ <= 65535U; }
  bool is_32bit() const { return asn_ > 65535U; }
  bool is_as_trans() const { return asn_ == 23456U; }
  bool is_private() const {
    return (64512U <= asn_ && asn_ <= 65534U) || (4200000000U <= asn_ && asn_ <= 4294967294U);
  }
  bool is_reserved() const {
    return
      // 0: Reserved (RFC7607)
      asn_ == 0
      // 64496-64511: Reserved for use in documentation and sample code (RFC5398)
      || (asn_ >> 4 == 4031U)
      // 65535: Reserved (RFC7300)
      || asn_ == 65535U
      // 65536-65551: Reserved for use in documentation and sample code (RFC5398)
      // 65552-131071: Reserved
      || (asn_ >> 16 == 1U)
      // 4294967295: Reserved (RFC7300)
      || asn_ == 4294967295U;
  }
  bool is_public() const { return !is_private() && !is_reserved(); }

private:
  uint32_t asn_;
};

} // namespace bgp
} // namespace parsebgp
