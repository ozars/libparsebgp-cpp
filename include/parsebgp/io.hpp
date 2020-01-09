#pragma once

#include <memory>

#include "parsebgp/utils.hpp"

namespace parsebgp {
namespace io {

class GzipStream : private utils::MutableOpaqueView<GzipStream> {
   public:
    GzipStream(utils::string_view path, size_t internal_buf_size) noexcept;
    ~GzipStream();
    GzipStream(const GzipStream&) = delete;
    GzipStream(GzipStream&&) = default;
    GzipStream& operator=(const GzipStream&) = delete;
    GzipStream& operator=(GzipStream&&) = default;

    size_t read(void *buffer, size_t length);
    bool good();
    bool eof();
    bool bad() const;
    int status_code() const;
    void clear_status_code();

   private:
    int status_code_;
    friend class utils::Impl;
};

}  // namespace io
}  // namespace parsebgp
