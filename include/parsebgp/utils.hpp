#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>

#ifdef PARSEBGP_CPP_LOG_ENABLED
#include <iostream>
namespace parsebgp {
namespace utils {

static inline auto& logger = std::cerr;

} // namespace utils
} // namespace parsebgp
#else
namespace parsebgp {
namespace utils {

struct Noop {};

template<typename T>
constexpr Noop operator<<(Noop, T) {
  return {};
}

static inline constexpr auto logger = Noop{};

} // namespace utils
} // namespace parsebgp
#endif

#include <nonstd/expected.hpp>
#include <nonstd/span.hpp>
#include <nonstd/string_view.hpp>

namespace parsebgp {
namespace utils {

using nonstd::dynamic_extent;
using nonstd::expected;
using nonstd::make_unexpected;
using nonstd::span;
using nonstd::string_view;

using bytes_view = span<const uint8_t>;
using ip_view = span<const uint8_t>; // TODO: Make this something lijke variant.
using ipv4_view = span<const uint8_t, 4>;
using ipv6_view = span<const uint8_t, 16>;

/* enable_if doesn't work in nondeduced contexts, so use sfinae_t/sfinae_v_t instead. */
template<typename T, typename... Ts>
using sfinae_t = T;

template<typename T, bool... Bs>
using sfinae_v_t = sfinae_t<T, typename std::enable_if<Bs>::type...>;

template<typename SelfT, typename CPtrT>
class CPtrView {
public:
  using CPtr = CPtrT;

  CPtrView() = delete;
  bool is_cptr_null() { return cptr_; }

private:
  friend SelfT;

  using BaseView = CPtrView;

  // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
  CPtrView(CPtr cptr) : cptr_(cptr) {}
  ~CPtrView() = default;
  CPtrView(const CPtrView&) = default;
  CPtrView(CPtrView&& other) noexcept : cptr_(other.cptr_) { other.cptr_ = nullptr; }
  CPtrView& operator=(const CPtrView&) = default;
  CPtrView& operator=(CPtrView&& other) noexcept {
    cptr_ = other.cptr_;
    other.cptr_ = nullptr;
    return *this;
  }

  CPtr cptr() const { return cptr_; }

  CPtr cptr_;
};

#define LIBPARSEBGP_CPP_CRTP_SELF_ONLY_ACCESS(ClassName_, SelfType_)                               \
  friend SelfType_;                                                                                \
  constexpr ClassName_() {}                                                                        \
  ~ClassName_() = default;                                                                         \
  ClassName_(const ClassName_&) = default;                                                         \
  ClassName_(ClassName_&&) = default;                                                              \
  ClassName_& operator=(const ClassName_&) = default;                                              \
  ClassName_& operator=(ClassName_&&) = default;

template<typename SelfT>
class FlagsClass {
public:
  bool operator==(const SelfT& rhs) const {
    return static_cast<const SelfT&>(*this).value() == static_cast<const SelfT&>(rhs)->value();
  }

  bool operator!=(const SelfT& rhs) const {
    return static_cast<const SelfT&>(*this).value() != static_cast<const SelfT&>(rhs)->value();
  }

private:
  LIBPARSEBGP_CPP_CRTP_SELF_ONLY_ACCESS(FlagsClass, SelfT)
};

template<typename SelfT>
class EnumClass {
public:
  bool operator==(const SelfT& rhs) const {
    return static_cast<const SelfT&>(this).value() == static_cast<const SelfT&>(rhs)->value();
  }

  bool operator!=(const SelfT& rhs) const {
    return static_cast<const SelfT&>(this).value() != static_cast<const SelfT&>(rhs)->value();
  }

private:
  LIBPARSEBGP_CPP_CRTP_SELF_ONLY_ACCESS(EnumClass, SelfT)
};

template<typename SelfT, typename ElementT, typename ElementCPtrT = typename ElementT::CPtr>
class CPtrRange {
public:
  using Element = ElementT;
  using ElementCPtr = ElementCPtrT;

  class Iterator : public CPtrView<Iterator, ElementCPtr> {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = Element;
    using difference_type = std::ptrdiff_t;
    using pointer = Element*;     // dunno why this is necessary
    using reference = value_type; // no lvalue ref since it is a view already

    reference operator*() const { return { cptr() }; }

    Iterator operator+(difference_type n) const { return { SelfT::range_add(cptr(), n) }; }
    Iterator operator-(difference_type n) const { return { SelfT::range_subtract(cptr(), n) }; }

    Iterator& operator+=(difference_type n) { return (*this = *this + 1); }
    Iterator& operator-=(difference_type n) { return (*this = *this - 1); }

    Iterator& operator++() { return (*this += 1); }
    Iterator& operator--() { return (*this -= 1); }

    Iterator operator++(int) {
      Iterator it = *this;
      static_cast<void>(++*this);
      return it;
    }
    Iterator operator--(int) {
      Iterator it = *this;
      static_cast<void>(--*this);
      return it;
    }

    difference_type operator-(Iterator rhs) const {
      return SelfT::range_difference(cptr(), rhs.cptr());
    }

    reference operator[](difference_type n) { return *(*this + n); }

    bool operator==(Iterator rhs) const { return cptr() == rhs.cptr(); }
    bool operator!=(Iterator rhs) const { return cptr() != rhs.cptr(); }

    bool operator<(Iterator rhs) const { return cptr() < rhs.cptr(); }
    bool operator>(Iterator rhs) const { return cptr() > rhs.cptr(); }
    bool operator<=(Iterator rhs) const { return cptr() <= rhs.cptr(); }
    bool operator>=(Iterator rhs) const { return cptr() >= rhs.cptr(); }

  private:
    friend class CPtrRange;
    using typename CPtrView<Iterator, ElementCPtr>::BaseView;
    using typename CPtrView<Iterator, ElementCPtr>::CPtr;
    using CPtrView<Iterator, ElementCPtr>::cptr;

    // NOLINTNEXTLINE(google-explicit-constructor): Allow propagation of C pointer.
    Iterator(CPtr cptr) : BaseView(cptr) {}
  };

public:
  Iterator begin() const { return static_cast<const SelfT&>(*this).range_data(); }
  Iterator end() const { return begin() + size(); }
  std::size_t size() const { return static_cast<const SelfT&>(*this).range_size(); }

  Element operator[](int i) const { return begin()[i]; }

private:
  using BaseRange = CPtrRange;
  LIBPARSEBGP_CPP_CRTP_SELF_ONLY_ACCESS(CPtrRange, SelfT)
};

#undef LIBPARSEBGP_CPP_CRTP_SELF_ONLY_ACCESS

} // namespace utils
} // namespace parsebgp
