#include <parsebgp/bgp/opts.hpp>
#include <parsebgp_bgp_opts.h>

namespace parsebgp {
namespace bgp {

bool Options::marker_omitted() const {
  return cptr()->marker_omitted;
}

Options& Options::set_marker_omitted(bool value) {
  cptr()->marker_omitted = value;
  return *this;
}

bool Options::marker_copy() const {
  return cptr()->marker_copy;
}

Options& Options::set_marker_copy(bool value) {
  cptr()->marker_copy = value;
  return *this;
}

bool Options::asn_4_byte() const {
  return cptr()->asn_4_byte;
}

Options& Options::set_asn_4_byte(bool value) {
  cptr()->asn_4_byte = value;
  return *this;
}

bool Options::mp_reach_no_afi_safi_reserved() const {
  return cptr()->mp_reach_no_afi_safi_reserved;
}

Options& Options::set_mp_reach_no_afi_safi_reserved(bool value) {
  cptr()->mp_reach_no_afi_safi_reserved = value;
  return *this;
}

uint16_t Options::afi() const {
  return cptr()->afi;
}

Options& Options::set_afi(uint16_t value) {
  cptr()->afi = value;
  return *this;
}

uint8_t Options::safi() const {
  return cptr()->safi;
}

Options& Options::set_safi(uint8_t value) {
  cptr()->safi = value;
  return *this;
}

bool Options::path_attr_filter_enabled() const {
  return cptr()->path_attr_filter_enabled;
}

Options& Options::set_path_attr_filter_enabled(bool value) {
  cptr()->path_attr_filter_enabled = value;
  return *this;
}

bool Options::path_attr_filter(uint8_t index) const {
  return cptr()->path_attr_filter[index];
}

Options& Options::set_path_attr_filter(uint8_t index, bool value) {
  cptr()->path_attr_filter[index] = value;
  return *this;
}

bool Options::path_attr_raw_enabled() const {
  return cptr()->path_attr_raw_enabled;
}

Options& Options::set_path_attr_raw_enabled(bool value) {
  cptr()->path_attr_raw_enabled = value;
  return *this;
}

bool Options::path_attr_raw(uint8_t index) const {
  return cptr()->path_attr_raw[index];
}

Options& Options::set_path_attr_raw(uint8_t index, bool value) {
  cptr()->path_attr_raw[index] = value;
  return *this;
}

} // namespace bgp
} // namespace parsebgp
