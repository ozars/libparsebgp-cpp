#pragma once

#include <algorithm>
#include <functional>
#include <memory>

#include <parsebgp.hpp>
#include <parsebgp/utils.hpp>

extern "C" typedef struct gzFile_s* gzFile;

namespace parsebgp {
namespace io {

class GzipStream : public utils::CPtrView<GzipStream, gzFile> {
public:
  class Status : public utils::EnumClass<Status> {
  public:
    enum Value {
      OK = 0,
      STREAM_END = 1,
      NEED_DICT = 2,
      ERRNO = -1,
      STREAM_ERROR = -2,
      DATA_ERROR = -3,
      MEM_ERROR = -4,
      BUF_ERROR = -5,
      VERSION_ERROR = -6,
    };

    Status(Value value = OK) : value_(value) {}

    Value value() const { return value_; }
    bool is_valid() const {
      switch (value_) {
        case OK:
        case STREAM_END:
        case NEED_DICT:
        case ERRNO:
        case STREAM_ERROR:
        case DATA_ERROR:
        case MEM_ERROR:
        case BUF_ERROR:
        case VERSION_ERROR:
          return true;
      }
      return false;
    }
    bool is_ok() const { return value_ == OK; }
    bool is_stream_end() const { return value_ == STREAM_END; }
    bool is_need_dict() const { return value_ == NEED_DICT; }
    bool is_errno() const { return value_ == ERRNO; }
    bool is_stream_error() const { return value_ == STREAM_ERROR; }
    bool is_data_error() const { return value_ == DATA_ERROR; }
    bool is_mem_error() const { return value_ == MEM_ERROR; }
    bool is_buf_error() const { return value_ == BUF_ERROR; }
    bool is_version_error() const { return value_ == VERSION_ERROR; }

  private:
    Value value_;
  };

  GzipStream(utils::string_view path, size_t internal_buf_size);
  ~GzipStream();
  GzipStream(const GzipStream&) = delete;
  GzipStream(GzipStream&&) = default;
  GzipStream& operator=(const GzipStream&) = delete;
  GzipStream& operator=(GzipStream&&) = default;

  size_t read(void* buffer, size_t length);
  bool good();
  bool eof();
  bool bad() const;
  Status status() const;
  void clear_status();

private:
  Status status_;
};

/*
 * Mirrored ring buffer consists of two contiguous mapped memory regions mirroring each other.
 *
 * Let's denote buffer buf_.data() with B and capacity buf_.size() with C. Invariants:
 * - memcmp(D, B + C, C) is 0, i.e. both regions miror each other.
 * - left_ points to the beginning of the data available if there are any.
 * - right_ points to the beginning of the space available if there are any.
 * - B <= left_ < B + C
 * - left_ <= right_ < B + 2 * C
 * - There are (right_ - left_) bytes available to read.
 * - There are (left_ - B - (right_ - left_)) bytes available to write.
 * - All pointers to B are invalidated if C is modified (e.g. reserve()).
 */
class MirroredRingBuffer {

  /* Create a buffer with *at least* capacity bytes. It will be ceiled to multiple of page size. */
  MirroredRingBuffer(size_t capacity)
    : buf_(mirror_create(capacity)), left_(buf_.data()), right_(buf_.data()) {}

  ~MirroredRingBuffer() {
    if (buf_.data()) mirror_destroy(buf_);
  }

  MirroredRingBuffer(const MirroredRingBuffer&) = delete;

  MirroredRingBuffer(MirroredRingBuffer&& other) noexcept
    : buf_(other.buf_), left_(other.left_), right_(other.right_) {
    other.buf_ = {};
    other.left_ = nullptr;
    other.right_ = nullptr;
  }

  MirroredRingBuffer& operator=(const MirroredRingBuffer&) = delete;

  MirroredRingBuffer& operator=(MirroredRingBuffer&& other) noexcept {
    if (buf_.data()) mirror_destroy(buf_);
    buf_ = other.buf_;
    left_ = other.left_;
    right_ = other.right_;
    other.buf_ = {};
    other.left_ = nullptr;
    other.right_ = nullptr;
    return *this;
  }

  bool is_null() const { return !buf_.data(); }

  size_t capacity() const { return buf_.size(); }

  bool reserve(size_t new_capacity) {
    if (buf_.size() < new_capacity) {
      MirroredRingBuffer new_buf(new_capacity);
      if (new_buf.is_null()) return false;
      auto in = prepare_read();
      auto out = new_buf.prepare_write();
      // assert(in.size() <= out.size());
      std::copy(in.data(), in.data() + in.size(), out.data());
      *this = std::move(new_buf);
    }
    return true;
  }

  size_t available_read() const { return right_ - left_; }

  const utils::span<uint8_t> prepare_read() const { return { left_, available_read() }; }

  void commit_read(size_t bytes) {
    // assert(left_ + bytes <= right_);
    left_ += bytes;

    if (left_ >= buf_.data() + buf_.size()) {
      left_ -= buf_.size();
      right_ -= buf_.size();
    }
  }

  size_t available_write() const { return left_ - buf_.data() - available_read(); }

  utils::span<uint8_t> prepare_write() { return { right_, available_write() }; }

  void commit_write(size_t bytes) {
    // assert(right_ + bytes < buf_.data() + 2 * buf_.size());
    right_ += bytes;
  }

private:
  static utils::span<uint8_t> mirror_create(size_t capacity);
  static void mirror_destroy(utils::span<uint8_t> buf) noexcept;

  utils::span<uint8_t> buf_;
  uint8_t* left_;
  uint8_t* right_;
};

using ReaderResultType = const utils::expected<std::reference_wrapper<Message>, Error>;

static inline ReaderResultType identity_transform(ReaderResultType msg) {
  return msg;
};

template<typename Stream, Message::Type::Value message_type, auto transformer = identity_transform>
class Reader {
public:
  class Status {
  public:
    enum Value {
      OK,
      STREAM_FINISHED,
      FINISHED,
      STREAM_ERROR,
      DECODER_ERROR,
      MEMORY_FAILURE,
    };

    Status(Value value = OK) : value_(value), inner_error_({}) {}
    Status(typename Stream::Status stream_error_)
      : value_(STREAM_ERROR), inner_error_(stream_error_) {}
    Status(parsebgp::Error decoder_error_) : value_(DECODER_ERROR), inner_error_(decoder_error_) {}
    Status& operator=(Value value) {
      value_ = value;
      inner_error_ = {};
    }
    Status& operator=(typename Stream::Status stream_error_) {
      value_ = STREAM_ERROR;
      inner_error_ = stream_error_;
      return *this;
    }
    Status& operator=(parsebgp::Error decoder_error_) {
      value_ = DECODER_ERROR;
      inner_error_ = decoder_error_;
      return *this;
    }

    bool is_ok() const { return value_ == OK; }
    bool is_stream_finished() const { return value_ == STREAM_FINISHED; }
    bool is_finished() const { return value_ == FINISHED; }
    bool is_stream_error() const { return value_ == STREAM_ERROR; }
    bool is_decoder_error() const { return value_ == DECODER_ERROR; }
    bool is_memory_failure() const { return value_ == MEMORY_FAILURE; }

    bool not_finished() const { return is_ok() || is_stream_finished(); }

    typename Stream::Status to_stream_error() const {
      // assert(is_stream_error());
      return inner_error_.stream_;
    }

    parsebgp::Error to_decoder_error() const {
      // assert(is_stream_error());
      return inner_error_.stream_;
    }

  private:
    Value value_;
    union {
      typename Stream::Status stream_;
      parsebgp::Error decoder_;
    } inner_error_;
  };

  /* TODO: Add buffer as template parameter. */
  using Buffer = MirroredRingBuffer;
  using ResultType = ReaderResultType;
  using TransformedResultType = decltype(transformer(std::declval<ResultType>()));

  struct Sentinel;

  class Iterator {
  public:
    Iterator& operator++() {
      reader_->decode_one();
      return *this;
    }

    TransformedResultType operator*() { return reader_->message(); }
    bool operator==(const Iterator& rhs) const { return reader_ == rhs->reader_; }
    bool operator!=(const Iterator& rhs) const { return reader_ != rhs->reader_; }
    bool operator==(const Sentinel&) const { return !reader_.status_.not_finished(); }
    bool operator!=(const Sentinel&) const { return reader_.status_.not_finished(); }

  private:
    friend class Reader;
    Iterator(Reader* reader) : reader_(reader) {}
    Reader* reader_;
  };

  struct Sentinel {
    bool operator==(const Iterator& rhs) const { return rhs == *this; }
    bool operator!=(const Iterator& rhs) const { return rhs != *this; }
  };

  Reader(Stream stream, Options options = {}, size_t buffer_size = 32678) : started_(false) {}

  void decode_one() {
    if (status_.is_ok()) {
      auto out = buffer_.prepare_read();
      if (out.size() != 0) {
        bool already_got_partial = false;
        do {
          message_.clear();
          auto ret = message_.decode(options_, message_type, out.data(), out.size());
          if (ret) {
            buffer_.commit_read(ret.value());
            break;
          } else if (ret.error().is_partial_msg() && !status_.is_stream_finished()) {
            if (already_got_partial) {
              constexpr size_t growth_factor = 2;
              buffer_.reserve(buffer_.capacity() * growth_factor);
            } else {
              already_got_partial = true;
            }
            fill_buffer();
            out = buffer_.prepare_read();
            continue;
          } else {
            status_ = ret.error();
            break;
          }
        } while (true);
      } else {
        status_ = Status::FINISHED;
      }
    }
  }

  TransformedResultType message() {
    if (!started_) {
      started_ = true;
      fill_buffer();
      if (status_.not_finished()) decode_one();
    }
    if (status_.not_finished()) return transformer(ResultType(message_));
    return transformer(ResultType(status_));
  }

  Status status() const { return status_; }
  void clear_status() const { status_ = Status::OK; }

  Iterator begin() { return Iterator(this); }
  Sentinel end() { return {}; }

private:
  void fill_buffer() {
    // assert(status_.is_ok());
    auto in = buffer_.prepare_write();
    auto bytes_read = stream_.read(in.data(), in.size());
    if (stream_.good()) {
      buffer_.commit_write(bytes_read);
    } else if (stream_.eof()) {
      buffer_.commit_write(bytes_read);
      status = Status::STREAM_FINISHED;
    } else {
      // assert(stream_.bad());
      status_ = stream_.status_;
    }
  };

  bool started_;
  Stream stream_;
  Buffer buffer_;
  Options options_;
  Message message_;
  Status status_;
};

} // namespace io
} // namespace parsebgp
