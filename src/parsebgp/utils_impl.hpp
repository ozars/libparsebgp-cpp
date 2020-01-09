#pragma once

#include <type_traits>

#include "parsebgp/utils.hpp"

namespace parsebgp {
namespace utils {

class Impl {
public:
  template<typename T, typename Arg>
  static T create_enum(Arg&& arg) {
    using Value = typename T::Value;
    static_assert(sizeof(std::underlying_type_t<Value>) >= sizeof(Arg));
    return T(static_cast<Value>(std::forward<Arg>(arg)));
  }

  template<typename T, typename... Args>
  static T create(Args&&... args) {
    if constexpr (std::is_base_of_v<EnumClass<T>, T>) {
      static_assert(sizeof...(Args) == 1);
      return create_enum<T>(std::forward<Args>(args)...);
    } else {
      return T(std::forward<Args>(args)...);
    }
  }

  template<typename R, typename T>
  static R cptr(T& t) {
    return static_cast<R>(t.cptr());
  }

  template<typename R, typename T>
  static R cptr(T* t) {
    return static_cast<R>(t->cptr());
  }
};

} // namespace utils
} // namespace parsebgp
