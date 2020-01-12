#include <cassert>

#include <parsebgp.h>
#include <parsebgp.hpp>

namespace parsebgp {

Message::Message() : BaseView(parsebgp_create_msg()) {}

Message::~Message() {
  parsebgp_destroy_msg(cptr());
}

utils::expected<size_t, Error> decode(const Options& opts, Type type, const void* buf, size_t len) {
  auto e = Error::Value(parsebgp_decode_msg(*opts.cptr(), type, cptr(), buf, &len));
  if (e.is_ok()) return len;
  return e;
}

void Message::clear() {
  parsebgp_clear_msg(cptr());
}

void Message::dump() const {
  parsebgp_dump_msg(cptr());
}

const mrt::Message Message::to_mrt() const {
  return mrt::Message(cptr()->types.mrt);
}

} // namespace parsebgp
