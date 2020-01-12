#pragma once

#include <cstdint>

#include <parsebgp/utils.hpp>

extern "C" struct parsebgp_bgp_opts;

namespace parsebgp {
namespace bgp {

class Options : public utils::CPtrView<Options, parsebgp_bgp_opts*> {
public:
  Options(CPtr cptr) : BaseView(cptr) {}

  bool marker_omitted() const;
  Options& set_marker_omitted(bool value);

  bool marker_copy() const;
  Options& set_marker_copy(bool value);

  bool asn_4_byte() const;
  Options& set_asn_4_byte(bool value);

  bool mp_reach_no_afi_safi_reserved() const;
  Options& set_mp_reach_no_afi_safi_reserved(bool value);

  uint16_t afi() const;
  Options& set_afi(uint16_t value);

  uint8_t safi() const;
  Options& set_safi(uint8_t value);

  bool path_attr_filter_enabled() const;
  Options& set_path_attr_filter_enabled(bool value);

  bool path_attr_filter(uint8_t index) const;
  Options& set_path_attr_filter(uint8_t index, bool value);

  bool path_attr_raw_enabled() const;
  Options& set_path_attr_raw_enabled(bool value);

  bool path_attr_raw(uint8_t index) const;
  Options& set_path_attr_raw(uint8_t index, bool value);
};

} // namespace bgp
} // namespace parsebgp
