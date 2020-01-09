#pragma once

// utils.hpp

namespace parsebgp {
namespace utils {

template <typename T>
struct CPtr;

template <typename T>
struct OwnedCPtr;

template <typename T>
class OpaqueView;

template <typename T>
class OwnedOpaqueView;

template <typename Self>
class FlagClass;

template <typename Self>
class EnumClass;

template <typename Self>
class IteratorClass;

template <typename Self>
class RangeClass;

}  // namespace utils
}  // namespace parsebgp

// io.hpp

namespace parsebgp {
namespace io {

class GzipStream;

}  // namespace io

namespace utils {

extern template class OwnedCPtr<io::GzipStream>;

}
}  // namespace parsebgp
