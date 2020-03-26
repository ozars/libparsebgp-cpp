#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>

#include <parsebgp.hpp>
#include <parsebgp/utils.hpp>

extern "C" typedef struct gzFile_s* gzFile; // NOLINT(modernize-use-using)

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

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    Status(Value value = OK) : value_(value) {}

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    operator Value() const { return value_; }

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

class Bzip2Stream : public utils::CPtrView<Bzip2Stream, void*> {
public:
  class Status : public utils::EnumClass<Status> {
  public:
    enum Value {
      OK = 0,
      RUN_OK = 1,
      FLUSH_OK = 2,
      FINISH_OK = 3,
      STREAM_END = 4,
      SEQUENCE_ERROR = -1,
      PARAM_ERROR = -2,
      MEM_ERROR = -3,
      DATA_ERROR = -4,
      DATA_ERROR_MAGIC = -5,
      IO_ERROR = -6,
      UNEXPECTED_EOF = -7,
      OUTBUFF_FULL = -8,
      CONFIG_ERROR = -9,
    };

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    Status(Value value = OK) : value_(value) {}

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    operator Value() const { return value_; }

    Value value() const { return value_; }
    bool is_valid() const {
      switch (value_) {
        case OK:
        case RUN_OK:
        case FLUSH_OK:
        case FINISH_OK:
        case STREAM_END:
        case SEQUENCE_ERROR:
        case PARAM_ERROR:
        case MEM_ERROR:
        case DATA_ERROR:
        case DATA_ERROR_MAGIC:
        case IO_ERROR:
        case UNEXPECTED_EOF:
        case OUTBUFF_FULL:
        case CONFIG_ERROR:
          return true;
      }
      return false;
    }
    bool is_ok() const { return value_ == OK; }
    bool is_run_ok() const { return value_ == RUN_OK; }
    bool is_flush_ok() const { return value_ == FLUSH_OK; }
    bool is_finish_ok() const { return value_ == FINISH_OK; }
    bool is_stream_end() const { return value_ == STREAM_END; }
    bool is_sequence_error() const { return value_ == SEQUENCE_ERROR; }
    bool is_param_error() const { return value_ == PARAM_ERROR; }
    bool is_mem_error() const { return value_ == MEM_ERROR; }
    bool is_data_error() const { return value_ == DATA_ERROR; }
    bool is_data_error_magic() const { return value_ == DATA_ERROR_MAGIC; }
    bool is_io_error() const { return value_ == IO_ERROR; }
    bool is_unexpected_eof() const { return value_ == UNEXPECTED_EOF; }
    bool is_outbuff_full() const { return value_ == OUTBUFF_FULL; }
    bool is_config_error() const { return value_ == CONFIG_ERROR; }

  private:
    Value value_;
  };

  Bzip2Stream(utils::string_view path, size_t internal_buf_size);
  ~Bzip2Stream();
  Bzip2Stream(const Bzip2Stream&) = delete;
  Bzip2Stream(Bzip2Stream&&) = default;
  Bzip2Stream& operator=(const Bzip2Stream&) = delete;
  Bzip2Stream& operator=(Bzip2Stream&&) = default;

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
 * - There are (C - (right_ - left_)) bytes available to write.
 * - All pointers to B are invalidated if C is modified (e.g. reserve()).
 */
class MirroredRingBuffer {
public:
  /* Create a buffer with *at least* capacity bytes. It will be ceiled to multiple of page size. */
  explicit MirroredRingBuffer(size_t capacity)
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

  size_t capacity() const { return buf_.size() / 2; }

  bool reserve(size_t new_capacity) {
    if (capacity() < new_capacity) {
      MirroredRingBuffer new_buf(new_capacity);
      if (new_buf.is_null()) return false;
      auto in = prepare_read();
      auto out = new_buf.prepare_write();
      assert(in.size() <= out.size());
      std::copy(in.data(), in.data() + in.size(), out.data());
      *this = std::move(new_buf);
    }
    return true;
  }

  size_t available_read() const { return right_ - left_; }

  const utils::span<uint8_t> prepare_read() const { return { left_, available_read() }; }

  void commit_read(size_t bytes) {
    assert(left_ + bytes <= right_);
    left_ += bytes;

    if (left_ >= buf_.data() + capacity()) {
      left_ -= capacity();
      right_ -= capacity();
    }
  }

  size_t available_write() const { return capacity() - available_read(); }

  utils::span<uint8_t> prepare_write() { return { right_, available_write() }; }

  void commit_write(size_t bytes) {
    assert(right_ + bytes < buf_.data() + 2 * capacity());
    utils::logger << "Written " << bytes << " bytes into the buffer.\n";
    right_ += bytes;
  }

private:
  static utils::span<uint8_t> mirror_create(size_t capacity);
  static void mirror_destroy(utils::span<uint8_t> buf) noexcept;

  utils::span<uint8_t> buf_;
  uint8_t* left_;
  uint8_t* right_;
};

template<typename Stream>
class ReaderStatus {
public:
  using StreamStatus = typename std::decay_t<Stream>::Status;

  enum Value {
    OK,
    STREAM_FINISHED,
    FINISHED,
    STREAM_ERROR,
    DECODER_ERROR,
    MEMORY_FAILURE,
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  ReaderStatus(Value value = OK) : value_(value), inner_error_({}) {}

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  operator Value() const { return value_; }

  explicit ReaderStatus(StreamStatus stream_error_)
    : value_(STREAM_ERROR), inner_error_(stream_error_) {}
  explicit ReaderStatus(parsebgp::Error decoder_error_)
    : value_(DECODER_ERROR), inner_error_(decoder_error_) {}
  ReaderStatus& operator=(Value value) {
    value_ = value;
    inner_error_ = {};
    return *this;
  }
  ReaderStatus& operator=(StreamStatus stream_error_) {
    value_ = STREAM_ERROR;
    inner_error_.stream_ = stream_error_;
    return *this;
  }
  ReaderStatus& operator=(parsebgp::Error decoder_error_) {
    value_ = DECODER_ERROR;
    inner_error_.decoder_ = decoder_error_;
    return *this;
  }

  Value value() const { return value_; }
  bool is_ok() const { return value_ == OK; }
  bool is_stream_finished() const { return value_ == STREAM_FINISHED; }
  bool is_finished() const { return value_ == FINISHED; }
  bool is_stream_error() const { return value_ == STREAM_ERROR; }
  bool is_decoder_error() const { return value_ == DECODER_ERROR; }
  bool is_memory_failure() const { return value_ == MEMORY_FAILURE; }

  bool not_finished() const { return is_ok() || is_stream_finished(); }

  StreamStatus to_stream_error() const {
    assert(is_stream_error());
    return inner_error_.stream_;
  }

  parsebgp::Error to_decoder_error() const {
    assert(is_decoder_error());
    return inner_error_.stream_;
  }

private:
  Value value_;
  union {
    StreamStatus stream_;
    parsebgp::Error decoder_;
  } inner_error_;
};

template<typename Stream, typename T>
using ReaderTransformOutput = utils::expected<T, ReaderStatus<Stream>>;

template<typename Stream>
using ReaderTransformInput = ReaderTransformOutput<Stream, std::reference_wrapper<const Message>>;

template<typename Stream>
static ReaderTransformInput<Stream> identity_transform(ReaderTransformInput<Stream> msg) {
  return msg;
};

template<typename Stream, Message::Type::Value message_type, typename Transformer>
class Reader {
public:
  /* TODO: Add buffer as template parameter. */
  using Buffer = MirroredRingBuffer;
  using Status = ReaderStatus<Stream>;
  using TransformInput = ReaderTransformInput<Stream>;
  using TransformOutput = decltype(std::declval<Transformer>()(std::declval<TransformInput>()));

  struct Sentinel;

  class Iterator {
  public:
    Iterator& operator++() {
      reader_->decode_one();
      return *this;
    }

    TransformOutput operator*() { return reader_->message(); }
    bool operator==(const Iterator& rhs) const { return reader_ == rhs->reader_; }
    bool operator!=(const Iterator& rhs) const { return reader_ != rhs->reader_; }
    bool operator==(const Sentinel&) const { return !reader_->status_.not_finished(); }
    bool operator!=(const Sentinel&) const { return reader_->status_.not_finished(); }

  private:
    friend class Reader;
    explicit Iterator(Reader* reader) : reader_(reader) {}
    Reader* reader_;
  };

  struct Sentinel {
    bool operator==(const Iterator& rhs) const { return rhs == *this; }
    bool operator!=(const Iterator& rhs) const { return rhs != *this; }
  };

  explicit Reader(Stream&& stream,
                  Options options = {},
                  size_t buffer_size = 32678,
                  Transformer transformer = identity_transform<Stream>)
    : started_(false)
    , stream_(std::forward<Stream>(stream))
    , buffer_(buffer_size)
    , options_(std::move(options))
    , transformer_(std::forward<Transformer>(transformer)) {
    if (buffer_.is_null()) status_ = Status::MEMORY_FAILURE;
  }

  void decode_one() {
    if (status_.not_finished()) {
      auto out = buffer_.prepare_read();
      bool already_got_partial = false;
      while (true) {
        message_.clear();
        auto ret = message_.decode(options_, message_type, out.data(), out.size());
        if (ret) {
          buffer_.commit_read(ret.value());
          break;
        } else if (ret.error().is_partial_msg() && !status_.is_stream_finished()) {
          if (!already_got_partial) {
            already_got_partial = true;
          } else {
            constexpr size_t growth_factor = 2;
            buffer_.reserve(buffer_.capacity() * growth_factor);
          }
          fill_buffer();
          out = buffer_.prepare_read();
          continue;
        } else if (ret.error().is_partial_msg() && out.size() == 0) {
          status_ = Status::FINISHED;
          break;
        } else {
          status_ = ret.error();
          break;
        }
      }
    }
  }

  TransformOutput message() {
    if (!started_) {
      started_ = true;
      fill_buffer();
      if (status_.not_finished()) decode_one();
    }
    if (status_.not_finished()) return transformer_(std::cref(message_));
    return transformer_(utils::make_unexpected(status_));
  }

  Message release_message() {
    Message new_msg;
    swap(new_msg, message_);
    return new_msg;
  }

  Status status() const { return status_; }
  // void clear_status() const { status_ = Status::OK; }

  Iterator begin() { return Iterator(this); }
  Sentinel end() { return {}; }

private:
  void fill_buffer() {
    assert(status_.is_ok());
    auto in = buffer_.prepare_write();
    auto bytes_read = stream_.read(in.data(), in.size());
    if (stream_.good()) {
      buffer_.commit_write(bytes_read);
    } else if (stream_.eof()) {
      buffer_.commit_write(bytes_read);
      status_ = Status::STREAM_FINISHED;
    } else {
      assert(stream_.bad());
      status_ = stream_.status();
    }
  };

  bool started_;
  Stream stream_;
  Buffer buffer_;
  Options options_;
  Message message_;
  Status status_;
  std::reference_wrapper<std::remove_reference_t<Transformer>> transformer_;
};

template<typename Stream>
static const ReaderTransformOutput<Stream, mrt::Message> mrt_transform(
  ReaderTransformInput<Stream> ret) {
  if (ret.has_value()) return ret.value().get().to_mrt();
  return utils::make_unexpected(ret.error());
};

template<typename Stream>
using MrtReader = Reader<Stream, Message::Type::MRT, decltype((mrt_transform<Stream>))>;

template<typename Stream>
MrtReader<Stream> mrt_reader(Stream&& stream, Options options = {}, size_t buffer_size = 32678) {
  return MrtReader<Stream>(
    std::forward<Stream>(stream), std::move(options), buffer_size, mrt_transform<Stream>);
}

} // namespace io
} // namespace parsebgp
