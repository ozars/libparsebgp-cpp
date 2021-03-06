#pragma once

#include <parsebgp/bgp.hpp>
#include <parsebgp/error.hpp>
#include <parsebgp/mrt.hpp>
#include <parsebgp/opts.hpp>
#include <parsebgp/utils.hpp>

extern "C" struct parsebgp_msg;

namespace parsebgp {

class Message : public utils::CPtrView<Message, parsebgp_msg*> {
public:
  class Type : public utils::EnumClass<Type> {
  public:
    enum Value {
      INVALID = 0,
      BGP = 1,
      BMP = 2,
      MRT = 3,
    };

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    constexpr Type(Value value) : value_(value) {}

    // NOLINTNEXTLINE(google-explicit-constructor): Enum class.
    constexpr operator Value() const { return value_; }

    bool is_valid() const {
      switch (value_) {
        case BGP:
        case BMP:
        case MRT:
          return true;
        case INVALID:
          return false;
      }
      return false;
    }
    Value value() const { return value_; }
    bool is_bgp() const { return value_ == BGP; }
    bool is_bmp() const { return value_ == BMP; }
    bool is_mrt() const { return value_ == MRT; }

  private:
    Value value_;
  };

  Message();
  ~Message();
  Message(const Message&) = delete;
  Message(Message&&) = default;
  Message& operator=(const Message&) = delete;
  Message& operator=(Message&&) = default;

  Type type() const;
  utils::expected<size_t, Error> decode(const Options& opts,
                                        Type type,
                                        const uint8_t* buf,
                                        size_t len);
  void clear();
  void dump() const;

  mrt::Message to_mrt() const;
};

inline void swap(Message m1, Message m2) {
  Message t = std::move(m1);
  m1 = std::move(m2);
  m2 = std::move(t);
}

} // namespace parsebgp
