#pragma once

#include <cstddef>

#include <parsebgp/utils.hpp>

namespace parsebgp {

class Error : utils::EnumClass<Error> {
public:
  enum Value {
    OK = 0,
    PARTIAL_MSG = -1,
    INVALID_MSG = -2,
    NOT_IMPLEMENTED = -3,
    MEMORY_FAILURE = -4,
    TRUNCATED_MSG = -5,
  };

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  Error(Value value = OK) : value_(value) {}

  // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
  operator Value() const { return value_; }

  Value value() const { return value_; }
  bool is_valid() const {
    switch (value_) {
      case OK:
      case PARTIAL_MSG:
      case INVALID_MSG:
      case NOT_IMPLEMENTED:
      case MEMORY_FAILURE:
      case TRUNCATED_MSG:
        return true;
    }
    return false;
  }
  bool is_ok() const { return value_ == OK; }
  bool is_partial_msg() const { return value_ == PARTIAL_MSG; }
  bool is_invalid_msg() const { return value_ == INVALID_MSG; }
  bool is_not_implemented() const { return value_ == NOT_IMPLEMENTED; }
  bool is_memory_failure() const { return value_ == MEMORY_FAILURE; }
  bool is_truncated_msg() const { return value_ == TRUNCATED_MSG; }

  utils::string_view to_string() const;

private:
  Value value_;
};

inline utils::string_view to_string(Error code) {
  return code.to_string();
}

} // namespace parsebgp
