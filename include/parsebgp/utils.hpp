#pragma once

#include <memory>
#include <type_traits>

#include <nonstd/optional.hpp>
#include <nonstd/span.hpp>
#include <nonstd/string_view.hpp>

namespace parsebgp {
namespace utils {

using nonstd::dynamic_extent;
using nonstd::nullopt;
using nonstd::optional;
using nonstd::span;
using nonstd::string_view;

template<typename T, size_t Extent = dynamic_extent>
using array_view = span<const T, Extent>;

using ipv4_view = array_view<uint8_t, 4>;
using ipv6_view = array_view<uint8_t, 16>;

template<typename SelfT, typename CPtrT>
class CPtrView {
private:
  friend SelfT;
  friend class Impl;

  CPtrView() = delete;
  ~CPtrView() = default;
  CPtrView(const CPtrView&) = default;
  CPtrView(CPtrView&&) = default;
  CPtrView& operator=(const CPtrView&) = default;
  CPtrView& operator=(CPtrView&&) = default;

  using Self = SelfT;
  using BaseView = CPtrView;
  using CPtr = CPtrT;
  using ConstCPtr = std::add_pointer_t<std::add_const_t<std::remove_pointer_t<CPtrT>>>;

  CPtrView(CPtr cptr) noexcept : cptr_(cptr) {}
  CPtr cptr() noexcept { return cptr_; }
  ConstCPtr cptr() const noexcept { return cptr_; }

  CPtr cptr_;
};

template<typename Self>
class FlagClass {
public:
  bool operator==(const Self& rhs) const {
    return static_cast<const Self&>(*this).value() == static_cast<const Self&>(rhs)->value();
  }

  bool operator!=(const Self& rhs) const {
    return static_cast<const Self&>(*this).value() != static_cast<const Self&>(rhs)->value();
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

template<typename Self>
class EnumClass {
public:
  bool operator==(const Self& rhs) const {
    return static_cast<const Self&>(this).value() == static_cast<const Self&>(rhs)->value();
  }

  bool operator!=(const Self& rhs) const {
    return static_cast<const Self&>(this).value() != static_cast<const Self&>(rhs)->value();
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

template<typename Self>
class IteratorClass {};

template<typename Self>
class RangeClass {};

} // namespace utils
} // namespace parsebgp
