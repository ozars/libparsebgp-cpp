#pragma once

#include <memory>
#include <nonstd/optional.hpp>
#include <nonstd/span.hpp>
#include <nonstd/string_view.hpp>

#define PARSEBGP_UTILS_NODISCARD [[nodiscard]]

namespace parsebgp {
namespace utils {

using nonstd::dynamic_extent;
using nonstd::nullopt;
using nonstd::optional;
using nonstd::span;
using nonstd::string_view;

template <typename T, size_t Extent = dynamic_extent>
using array_view = span<const T, Extent>;

template <typename Self>
class OpaqueView {
   private:
    friend Self;
    friend class Impl;

    OpaqueView() = delete;
    ~OpaqueView() = default;
    OpaqueView(const OpaqueView&) = default;
    OpaqueView(OpaqueView&&) = default;
    OpaqueView& operator=(const OpaqueView&) = default;
    OpaqueView& operator=(OpaqueView&&) = default;

    using OpaqueBase = OpaqueView<Self>;
    using CPtr = const void*;

    OpaqueView(CPtr cptr) noexcept : cptr_(cptr) {}

    PARSEBGP_UTILS_NODISCARD
    auto cptr() const noexcept { return cptr_; }

    CPtr cptr_;
};

template <typename Self>
class MutableOpaqueView {
   private:
    friend Self;
    friend class Impl;

    MutableOpaqueView() = delete;
    ~MutableOpaqueView() = default;
    MutableOpaqueView(const MutableOpaqueView&) = default;
    MutableOpaqueView(MutableOpaqueView&&) = default;
    MutableOpaqueView& operator=(const MutableOpaqueView&) = default;
    MutableOpaqueView& operator=(MutableOpaqueView&&) = default;

    using OpaqueBase = MutableOpaqueView<Self>;
    using CPtr = void*;

    MutableOpaqueView(CPtr cptr) noexcept : cptr_(cptr) {}

    PARSEBGP_UTILS_NODISCARD
    auto cptr() noexcept { return cptr_; }

    PARSEBGP_UTILS_NODISCARD
    auto cptr() const noexcept { return cptr_; }

    CPtr cptr_;
};

template <typename Self>
class FlagClass {
   public:
    PARSEBGP_UTILS_NODISCARD
    bool operator==(const Self& rhs) const {
        return static_cast<const Self&>(*this).value() ==
               static_cast<const Self&>(rhs)->value();
    }

    PARSEBGP_UTILS_NODISCARD
    bool operator!=(const Self& rhs) const {
        return static_cast<const Self&>(*this).value() !=
               static_cast<const Self&>(rhs)->value();
    }

   private:
    friend Self;
    friend class Impl;

    FlagClass() = default;
    ~FlagClass() = default;
    FlagClass(const FlagClass&) = default;
    FlagClass(FlagClass&&) = default;
    FlagClass& operator=(const FlagClass&) = default;
    FlagClass& operator=(FlagClass&&) = default;
};

template <typename Self>
class EnumClass {
   public:
    PARSEBGP_UTILS_NODISCARD
    bool operator==(const Self& rhs) const {
        return static_cast<const Self&>(this).value() ==
               static_cast<const Self&>(rhs)->value();
    }

    PARSEBGP_UTILS_NODISCARD
    bool operator!=(const Self& rhs) const {
        return static_cast<const Self&>(this).value() !=
               static_cast<const Self&>(rhs)->value();
    }

   private:
    friend Self;
    friend class Impl;

    EnumClass() = default;
    ~EnumClass() = default;
    EnumClass(const EnumClass&) = default;
    EnumClass(EnumClass&&) = default;
    EnumClass& operator=(const EnumClass&) = default;
    EnumClass& operator=(EnumClass&&) = default;
};

template <typename Self>
class IteratorClass {};

template <typename Self>
class RangeClass {};

}  // namespace utils
}  // namespace parsebgp
