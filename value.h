//
// Created by osboxes on 11/20/20.
//

#ifndef CPPLOX_VALUE_H
#define CPPLOX_VALUE_H
#include <variant>
// instead of just saying comma, TokenType::COMMA
using Value = std::variant<std::monostate, bool, double, std::string>;

std::ostream& operator<<(std::ostream& os, const Value& value);
#endif //CPPLOX_VALUE_H
