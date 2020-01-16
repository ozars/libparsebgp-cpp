#include <algorithm>
#include <new>

#include <parsebgp/opts.hpp>
#include <parsebgp_opts.h>

#include <parsebgp/bgp/opts.hpp>

namespace parsebgp {

//==============================================================================
// Options
//==============================================================================

Options::Options() : BaseView(new (std::nothrow) parsebgp_opts) {
  if (cptr()) parsebgp_opts_init(cptr());
}

Options::~Options() {
  delete cptr();
}

bool Options::ignore_not_implemented() const {
  return cptr()->ignore_not_implemented;
}

Options& Options::set_ignore_not_implemented(bool value) {
  cptr()->ignore_not_implemented = value;
  return *this;
}

bool Options::silence_not_implemented() const {
  return cptr()->silence_not_implemented;
}

Options& Options::set_silence_not_implemented(bool value) {
  cptr()->silence_not_implemented = value;
  return *this;
}

bool Options::ignore_invalid() const {
  return cptr()->ignore_invalid;
}

Options& Options::set_ignore_invalid(bool value) {
  cptr()->ignore_invalid = value;
  return *this;
}

bool Options::silence_invalid() const {
  return cptr()->silence_invalid;
}

Options& Options::set_silence_invalid(bool value) {
  cptr()->silence_invalid = value;
  return *this;
}

bgp::Options Options::bgp() const {
  return bgp::Options(&cptr()->bgp);
}

} // namespace parsebgp
