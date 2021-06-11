//
// Created by osboxes on 10/30/20.
//

#ifndef CPPLOX_TOKEN_H
#define CPPLOX_TOKEN_H

#include "value.h"
#include <string>

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  // EOF??
  eof

};
struct Token {
  // No constructor needed (at this point) because it is very "simple"
  // this is what C++ called "aggregate" initialization
  const TokenType type;
  const std::string lexeme;
  const Value literal;
  const std::size_t line;
};

std::ostream &operator<<(std::ostream &os, const TokenType &tt);
std::ostream &operator<<(std::ostream &os, const Token &t);

#endif // CPPLOX_TOKEN_H
