#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>

#include <nonstd/expected.hpp>
#include <nonstd/span.hpp>
#include <nonstd/string_view.hpp>

namespace parsebgp {
namespace utils {

using nonstd::dynamic_extent;
using nonstd::expected;
using nonstd::span;
using nonstd::string_view;

using ipv4_span = span<uint8_t, 4>;
using ipv6_span = span<uint8_t, 16>;

template<typename SelfT, typename CPtrT>
class CPtrView {
public:
  using CPtr = CPtrT;

  bool is_cptr_null() { return cptr_; }

private:
  friend SelfT;

  using BaseView = CPtrView;

  CPtrView() = delete;
  CPtrView(CPtr cptr) : cptr_(cptr) {}
  ~CPtrView() = default;
  CPtrView(const CPtrView&) = default;
  CPtrView(CPtrView&& other) noexcept : cptr_(other.cptr_) { other.cptr_ = nullptr; }
  CPtrView& operator=(const CPtrView&) = default;
  CPtrView& operator=(CPtrView&& other) noexcept {
    cptr_ = other.cptr_;
    other.cptr_ = nullptr;
  }

  CPtr cptr() { return cptr_; }
  const CPtr cptr() const { return cptr_; }
  // Note: Not returning const T* is intentional above: subspans can't be constructed with const T*.
  //       T is decltype(*cptr_).

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

private:
  template<bool IsConst>
  class BaseIterator : public CPtrView<BaseIterator<IsConst>, ElementCPtr> {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<IsConst, const Element, Element>;
    using difference_type = std::ptrdiff_t;
    using pointer = Element*;     // dunno why this is necessary
    using reference = value_type; // no lvalue ref since it is a view already

    reference operator*() { return { cptr() }; }
    const reference operator*() const { return { cptr() }; }

    BaseIterator operator+(difference_type n) const { return { SelfT::range_add(cptr(), n) }; }
    BaseIterator operator-(difference_type n) const { return { SelfT::range_subtract(cptr(), n) }; }

    BaseIterator& operator+=(difference_type n) { return (*this = *this + 1); }
    BaseIterator& operator-=(difference_type n) { return (*this = *this - 1); }

    BaseIterator& operator++() { return (*this += 1); }
    BaseIterator& operator--() { return (*this -= 1); }

    BaseIterator operator++(int) {
      BaseIterator it = *this;
      static_cast<void>(++*this);
      return it;
    }
    BaseIterator operator--(int) {
      BaseIterator it = *this;
      static_cast<void>(--*this);
      return it;
    }

    difference_type operator-(BaseIterator rhs) const {
      return SelfT::range_difference(cptr(), rhs.cptr());
    }

    reference operator[](difference_type n) { return *(*this + n); }
    const reference operator[](difference_type n) const { return *(*this + n); }

    bool operator==(BaseIterator rhs) const { return cptr() == rhs.cptr(); }
    bool operator!=(BaseIterator rhs) const { return cptr() != rhs.cptr(); }

    bool operator<(BaseIterator rhs) const { return cptr() < rhs.cptr(); }
    bool operator>(BaseIterator rhs) const { return cptr() > rhs.cptr(); }
    bool operator<=(BaseIterator rhs) const { return cptr() <= rhs.cptr(); }
    bool operator>=(BaseIterator rhs) const { return cptr() >= rhs.cptr(); }

  private:
    friend class Iterator;
    friend class ConstIterator;
    using typename CPtrView<BaseIterator, ElementCPtr>::BaseView;
    using typename CPtrView<BaseIterator, ElementCPtr>::CPtr;
    using CPtrView<BaseIterator, ElementCPtr>::cptr;

    BaseIterator(CPtr cptr) : BaseView(cptr) {}
  };

public:
  class Iterator : public BaseIterator</* IsConst = */ false> {
  private:
    friend class CPtrRange;
    using typename BaseIterator<false>::CPtr;
    using BaseIterator<false>::cptr;
    Iterator(CPtr cptr) : BaseIterator<false>(cptr) {}
  };

  class ConstIterator : public BaseIterator</* IsConst = */ true> {
  private:
    friend class CPtrRange;
    using typename BaseIterator<true>::CPtr;
    using BaseIterator<true>::cptr;
    ConstIterator(CPtr cptr) : BaseIterator<true>(cptr) {}
  };

  ConstIterator begin() const {
    return ConstIterator(static_cast<const SelfT&>(*this).range_data());
  }
  ConstIterator end() const { return begin() + size(); }
  std::size_t size() const { return static_cast<const SelfT&>(*this).range_size(); }

  const Element operator[](int i) const { return begin()[i]; }

private:
  using BaseRange = CPtrRange;
  LIBPARSEBGP_CPP_CRTP_SELF_ONLY_ACCESS(CPtrRange, SelfT)
};

#undef LIBPARSEBGP_CPP_CRTP_SELF_ONLY_ACCESS

} // namespace utils
} // namespace parsebgp
