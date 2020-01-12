#include <parsebgp/error.hpp>
#include <parsebgp_error.h>

namespace parsebgp {

//==============================================================================
// Error
//==============================================================================

static_assert(Error::OK == int(PARSEBGP_OK));
static_assert(Error::PARTIAL_MSG == int(PARSEBGP_PARTIAL_MSG));
static_assert(Error::INVALID_MSG == int(PARSEBGP_INVALID_MSG));
static_assert(Error::NOT_IMPLEMENTED == int(PARSEBGP_NOT_IMPLEMENTED));
static_assert(Error::MEMORY_FAILURE == int(PARSEBGP_MALLOC_FAILURE));
static_assert(Error::TRUNCATED_MSG == int(PARSEBGP_TRUNCATED_MSG));

utils::string_view Error::to_string() const {
  return parsebgp_strerror(parsebgp_error_t(value_));
}

} // namespace parsebgp
