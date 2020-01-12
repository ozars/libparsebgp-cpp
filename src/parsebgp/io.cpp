#include <zlib.h>

#include <parsebgp/io.hpp>

namespace parsebgp {
namespace io {

//==============================================================================
// io::GzipStream
//==============================================================================

GzipStream::GzipStream(utils::string_view path, size_t internal_buf_size)
  : BaseView(gzopen(path.data(), "rb")), status_(Status::Value(cptr() ? Z_OK : Z_MEM_ERROR)) {
  if (cptr()) status_ = Status::Value(gzbuffer(cptr(), internal_buf_size));
}

GzipStream::~GzipStream() {
  // FIXME: Signal failed destruction safely to user somehow (terminate?)
  if (cptr()) gzclose_r(cptr());
}

size_t GzipStream::read(void* buffer, size_t length) {
  int ret = gzread(cptr(), buffer, length);
  if (ret >= 0) return size_t(ret);
  status_ = Status::Value(ret);
  return 0;
}

bool GzipStream::good() {
  return !bad() && !eof();
}

bool GzipStream::eof() {
  return gzeof(cptr());
}

bool GzipStream::bad() const {
  return status_.is_ok();
}

auto GzipStream::status() const -> Status {
  return status_;
}

void GzipStream::clear_status() {
  status_ = Status::OK;
}

} // namespace io
} // namespace parsebgp
