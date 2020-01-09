#include "parsebgp/mrt.hpp"
#include "parsebgp/utils_impl.hpp"

#include "parsebgp_mrt.h"

namespace parsebgp {
namespace mrt {

using utils::Impl;

//==============================================================================
// mrt::Message
//==============================================================================

DECLARE_SELF(Message, parsebgp_mrt_msg_t*)

auto Message::type() const -> MessageType {
    return Impl{}.create<MessageType>(SELF->type);
}

uint16_t Message::subtype() const {
    return SELF->subtype;
}

uint32_t Message::length() const {
    return SELF->len;
}

utils::optional<table_dump_v2::Message> to_table_dump_v2() const {
    if (!type().is_table_dump_v2())
        return utils::nullopt;
    return Impl{}.create<table_dump_v2::Message>(cptr());
}

}  // namespace mrt
}  // namespace parsebgp
