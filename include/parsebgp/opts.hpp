#pragma once

#include <parsebgp/bgp/opts.hpp>
#include <parsebgp/utils.hpp>

extern "C" struct parsebgp_opts;

namespace parsebgp {

class Options : public utils::CPtrView<Options, parsebgp_opts*> {
public:
  Options();
  ~Options();
  Options(const Options& other) = delete;
  Options(Options&& other) = default;
  Options& operator=(const Options& other) = delete;
  Options& operator=(Options&& other) = default;

  bool ignore_not_implemented() const;
  Options& set_ignore_not_implemented(bool value);

  bool silence_not_implemented() const;
  Options& set_silence_not_implemented(bool value);

  bool ignore_invalid() const;
  Options& set_ignore_invalid(bool value);

  bool silence_invalid() const;
  Options& set_silence_invalid(bool value);

  bgp::Options bgp();
  const bgp::Options bgp() const;

  // bmp::Options& bmp();
  // const bmp::Options& bmp() const;

private:
  friend class Message;
  using BaseView::cptr;
};

} // namespace parsebgp
