#pragma once


#include <type_traits>

#include "parsebgp/utils.hpp"

namespace parsebgp {
namespace utils {

class Impl {
   public:
    template <typename T, typename... Args>
    static T create(Args&&... args) {
        return T{std::forward<Args>(args)...};
    }

    template <typename T, typename... Args>
    static typename T::OpaqueBase create_base(Args&&... args) {
        return typename T::OpaqueBase(
            typename T::CPtr{std::forward<Args>(args)...});
    }

    template <typename R, typename T>
    static R cptr(T& t) {
        return static_cast<R>(t.cptr());
    }

    template <typename R, typename T>
    static R cptr(T* t) {
        return static_cast<R>(t->cptr());
    }
};

template <typename T, typename = void>
struct add_const_ptr;

template <typename T>
struct add_const_ptr<T, std::enable_if_t<std::is_pointer_v<T>>> {
    using type = std::add_pointer_t<std::add_const_t<std::remove_pointer_t<T>>>;
};

template <typename T>
using add_const_ptr_t = typename add_const_ptr<T>::type;

#define DECLARE_SELF(ClassType, CType)                          \
    static auto self(ClassType *, void *ptr) {                  \
        return static_cast<CType>(ptr);                         \
    }                                                           \
    static auto self(const ClassType *, const void *ptr) {      \
        return static_cast<utils::add_const_ptr_t<CType>>(ptr); \
    }

#define SELF self(this, cptr())

#define DEBUG_ASSERT()


}  // namespace utils
}  // namespace parsebgp
