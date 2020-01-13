#include <zlib.h>

#include <parsebgp/io.hpp>

#include <sys/mman.h>
#include <unistd.h>

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
  return !status_.is_ok();
}

auto GzipStream::status() const -> Status {
  return status_;
}

void GzipStream::clear_status() {
  status_ = Status::OK;
}

//==============================================================================
// io::MirroredRingBuffer
//==============================================================================

utils::span<uint8_t> MirroredRingBuffer::mirror_create(size_t capacity) {
  int fd = -1;
  uint8_t* addr = nullptr;
  uint8_t* tmp = nullptr;

  size_t page_size = size_t(getpagesize());
  if (capacity % page_size) capacity += page_size - capacity % page_size;

  if (capacity >= 2 * capacity) return {};

  fd = memfd_create("temp", 0);
  if (fd == -1) return {};

  int ret = ftruncate(fd, capacity);
  if (ret) goto destroy;

  addr = (uint8_t*)mmap(nullptr, 2 * capacity, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (addr == MAP_FAILED) goto destroy;

  tmp = (uint8_t*)mmap(addr, capacity, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0);
  if (tmp != addr) goto destroy;

  tmp = (uint8_t*)mmap(
    addr + capacity, capacity, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0);
  if (tmp != addr + capacity) goto destroy;

  if (close(fd)) {
    fd = -1;
    goto destroy;
  }

  return { addr, 2 * capacity };

destroy:
  if (fd != -1) close(fd);
  if (addr && addr != MAP_FAILED) munmap(addr, 2 * capacity);
  return {};
}

void MirroredRingBuffer::mirror_destroy(utils::span<uint8_t> buf) noexcept {
  // TODO: terminate on error
  munmap(buf.data(), buf.size());
}

} // namespace io
} // namespace parsebgp
