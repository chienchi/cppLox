//
// Created by osboxes on 11/13/20.
//

#include "Value.h"
#include "Token.h"
#include <sstream>

// Keypoint: both 'os' and value are polymorphic types. The compiler/library
// switch/dispatch based on their "real" type at run time. So we don't need to
// write one version for each combination. There could be infinite number of
// different types for os.
std::ostream &operator<<(std::ostream &os, const Value &value) {
  struct visitor {
    std::ostream &os;
    explicit visitor(std::ostream &os) : os(os) {}

    std::ostream &operator()(std::monostate) {
      os << "nil";
      return os;
    }

    std::ostream &operator()(bool b) {
      os << std::boolalpha << b;
      return os;
    }

    std::ostream &operator()(double d) {
      os << d;
      return os;
    }

    std::ostream &operator()(const std::string &str) {
      os << str;
      return os;
    }
  };
  // magic is here 'if else' is inside here, it will check every visit has the
  // correct type
  return std::visit(visitor{os}, value);
};
