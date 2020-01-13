#include <cassert>

#include <parsebgp.h>
#include <parsebgp.hpp>

namespace parsebgp {

Message::Message() : BaseView(parsebgp_create_msg()) {}

Message::~Message() {
  parsebgp_destroy_msg(cptr());
}

utils::expected<size_t, Error> Message::decode(const Options& opts,
                                               Type type,
                                               const uint8_t* buf,
                                               size_t len) {
  Error e = Error::Value(
    parsebgp_decode(*opts.cptr(), parsebgp_msg_type_t(type.value()), cptr(), buf, &len));
  if (e.is_ok()) return len;
  return utils::make_unexpected(e);
}

void Message::clear() {
  parsebgp_clear_msg(cptr());
}

void Message::dump() const {
  parsebgp_dump_msg(cptr());
}

mrt::Message Message::to_mrt() const {
  return mrt::Message(cptr()->types.mrt);
}

} // namespace parsebgp
