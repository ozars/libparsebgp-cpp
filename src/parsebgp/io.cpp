#include "parsebgp/io.hpp"

#include <zlib.h>

#include "parsebgp/utils_impl.hpp"

namespace parsebgp {
namespace io {

//==============================================================================
// io::GzipStream
//==============================================================================

DECLARE_SELF(GzipStream, gzFile)

GzipStream::GzipStream(utils::string_view path,
                       size_t internal_buf_size) noexcept
    : OpaqueBase(CPtr{gzopen(path.data(), "rb")}), status_code_(Z_OK) {
    if (SELF) status_code_ = gzbuffer(SELF, internal_buf_size);
}

GzipStream::~GzipStream() {
    // FIXME: Handle failed destruction
    if (SELF) gzclose_r(SELF);
}

size_t GzipStream::read(void* buffer, size_t length) {
    int ret = gzread(SELF, buffer, length);
    if (ret >= 0) return size_t(ret);
    status_code_ = ret;
    return 0;
}

bool GzipStream::good() { return !bad() && !eof(); }

bool GzipStream::eof() { return gzeof(SELF); }

bool GzipStream::bad() const { return !cptr() || status_code_ != Z_OK; }

int GzipStream::status_code() const { return status_code_; }

void GzipStream::clear_status_code() { status_code_ = Z_OK; }

}  // namespace io
}  // namespace parsebgp
