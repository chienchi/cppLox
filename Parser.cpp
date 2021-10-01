//
// Created by osboxes on 3/12/21.
//

#include "Parser.h"
bool Parser::isAtEnd() { return current >= tokens.size(); }

Token Parser::consume(TokenType type, std::string message) {
  if (check(type))
    return advance();
  throw parser_error{message};
}

bool Parser::check(TokenType type) {
  if (isAtEnd())
    return false;
  return (peek().type == type);
}
Token Parser::peek() { return tokens[current]; }

Token Parser::advance() {
  if (!isAtEnd()) {
    current++;
  }
  return previous();
}
Token Parser::previous() { return tokens[current - 1]; }

Parser::parser_error::parser_error(std::string string) : message(string) {}
