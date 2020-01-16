#include <cassert>
#include <sys/mman.h>
#include <unistd.h>
#include <zlib.h>

#include <parsebgp/io.hpp>

namespace parsebgp {
namespace io {

//==============================================================================
// io::GzipStream
//==============================================================================

GzipStream::GzipStream(utils::string_view path, size_t internal_buf_size)
  : BaseView(gzopen(path.data(), "rb")), status_(Status::Value(cptr() ? Z_OK : Z_MEM_ERROR)) {
  if (cptr()) status_ = Status(Status::Value(gzbuffer(cptr(), internal_buf_size)));
}

GzipStream::~GzipStream() {
  if (cptr()) {
    int ret = gzclose_r(cptr());
    assert(ret == Z_OK);
  }
}

size_t GzipStream::read(void* buffer, size_t length) {
  int ret = gzread(cptr(), buffer, length);
  if (ret >= 0) return size_t(ret);
  status_ = Status(Status::Value(ret));
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

  auto page_size = size_t(getpagesize());
  if (capacity % page_size) capacity += page_size - capacity % page_size;

  if (capacity >= 2 * capacity) return {};

  fd = memfd_create("temp", 0);
  if (fd == -1) return {};

  int ret = ftruncate(fd, capacity);
  if (ret) goto fail;

  addr = static_cast<uint8_t*>(
    mmap(nullptr, 2 * capacity, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
  if (addr == MAP_FAILED) goto fail;

  tmp = static_cast<uint8_t*>(
    mmap(addr, capacity, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0));
  if (tmp != addr) goto fail;

  tmp = static_cast<uint8_t*>(
    mmap(addr + capacity, capacity, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0));
  if (tmp != addr + capacity) goto fail;

  if (close(fd)) {
    fd = -1;
    goto fail;
  }

  return { addr, 2 * capacity };

fail:
  if (fd != -1) {
    int ret = close(fd);
    assert(ret == 0);
  }
  if (addr && addr != MAP_FAILED) {
    int ret = munmap(addr, 2 * capacity);
    assert(ret == 0);
  }
  return {};
}

void MirroredRingBuffer::mirror_destroy(utils::span<uint8_t> buf) noexcept {
  int ret = munmap(buf.data(), buf.size());
  assert(ret == 0);
}

} // namespace io
} // namespace parsebgp
