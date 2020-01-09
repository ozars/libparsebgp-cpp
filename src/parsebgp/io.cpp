#include "parsebgp/io.hpp"

#include <zlib.h>

#include "parsebgp/utils_impl.hpp"

namespace parsebgp {
namespace io {

//==============================================================================
// io::GzipStream
//==============================================================================

GzipStream::GzipStream(utils::string_view path,
                       size_t internal_buf_size) noexcept
    : BaseView(gzopen(path.data(), "rb")), status_code_(Z_OK) {
    if (cptr()) status_code_ = gzbuffer(cptr(), internal_buf_size);
}

GzipStream::~GzipStream() {
    // FIXME: Handle failed destruction
    if (cptr()) gzclose_r(cptr());
}

size_t GzipStream::read(void* buffer, size_t length) {
    int ret = gzread(cptr(), buffer, length);
    if (ret >= 0) return size_t(ret);
    status_code_ = ret;
    return 0;
}

bool GzipStream::good() { return !bad() && !eof(); }

bool GzipStream::eof() { return gzeof(cptr()); }

bool GzipStream::bad() const { return !cptr() || status_code_ != Z_OK; }

int GzipStream::status_code() const { return status_code_; }

void GzipStream::clear_status_code() { status_code_ = Z_OK; }

}  // namespace io
}  // namespace parsebgp
