//
// Created by osboxes on 11/20/20.
//

#ifndef CPPLOX_SCANNER_H
#define CPPLOX_SCANNER_H
#include "Token.h"
#include "error.h"
#include <string>
#include <vector>

class Scanner {
public:
  Scanner(const std::string &s_source) : source(s_source){};

private:
  bool isAtEnd();
  char advance();
  void scanToken();
  void addToken(TokenType type);
  void addToken(TokenType type, std::string text);
  bool match(char expected);
  char peek();
  void string();
  bool isDigit(char c);
  void number(char c);
  char peekNext();
  bool isAlpha(char c);
  bool isAlphaNumeric(char c);
  void identifier();

  auto zeroOrder() {
    std::vector<std::string> stokens;
    // 0th approximation, scan from left to right, slice input
    // string every time we see a white space
    // 1. keep a record of current position
    // 2. terminate when current position >= .size()
    // 3. call string.substr(start,count)
    while (!isAtEnd()) {
      if (source[current] == ' ') {
        stokens.push_back(source.substr(start, current - start));
        start = current;
      }
      current++; // ToDO: can we remove this line??
    }
    return stokens;
  }

  void addStoken(TokenType type, std::string t) {
    tokens.push_back({type, t, {}, line});
  }
  char tadvance() {
    current++;
    return source[current - 1];
  }
  char cpeek() { return isAtEnd() ? '\0' : source[current]; }
  bool cmatch(char expected) {
    if (isAtEnd() or source[current] != expected) {
      return false;
    }
    current++;
    return true;
  }
  auto firstOrder() {
    // 1st order approximation, scan from left to right, slice input
    // string everytime we see a non-alphabet (in addition to white space).
    // 1. keep a record of starting and current position
    // 2. terminate when current > .size()
    // 3. call string.substr(start,count)
    // Q: Expected outcome???
    //    ==> "var", "language", "=", ???
    // Q: What is the definition of "non-alphabet", how exactly do we deal with
    // it? A: We need to treat space differently from other non-alphabet
    // char c = source[current];
    while (!isAtEnd()) {
      if (isspace(cpeek())) {
        tadvance();
        start = current;
      } else if (isalnum(cpeek())) {
        while (isalnum(cpeek())) {
          tadvance();
        }
        addStoken(TokenType::IDENTIFIER, source.substr(start, current - start));
        start = current;
      } else {
        if (cmatch('=')) {
          addStoken(TokenType::EQUAL, source.substr(start, current - start));
        } else {
          tadvance();
        }
      }
    }
    return tokens;
  }

  auto secondOrder() {
    // 2nd order approximation, deal with "" for string
    while (!isAtEnd()) {
      if (cpeek() == '"') {
        tadvance();
        start = current;
        while (!isAtEnd() and cpeek() != '"') {
          // move until next to '"' as a string
          tadvance();
        }
        if (start != current) {
          addStoken(TokenType::STRING, source.substr(start, current - start));
          tadvance(); // step into '"'
          start = current + 1;
        }
      } else {
        if (!std::isalnum(cpeek())) {

          if (std::isalnum(source[current - 1])) {
            addStoken(TokenType::IDENTIFIER,
                      source.substr(start, current - start));
          } else if (!std::isspace(source[current - 1])) {
            addStoken(TokenType::IDENTIFIER,
                      source.substr(start - 1, current - start + 1));
          }
          start = current + 1;
        } else {
          if (start > 0 and !std::isalnum(source[current - 1]) and
              !std::isspace(source[current - 1])) {
            addStoken(TokenType::IDENTIFIER,
                      source.substr(start - 1, current - start + 1));
          }
        }
      }
      tadvance();
    }
    return tokens;
  }

public:
  auto scanTokens() {
    while (!isAtEnd()) {
      start = current;
      scanToken();
    }

    // tokens.push_back(Token{TokenType::eof,"",Value{},line});
    return tokens;
    // return firstOrder();
  }

private:
  const std::string &source;
  std::size_t start = 0;
  std::size_t current = 0;
  std::size_t line = 1;
  std::vector<Token> tokens;
};

#endif // CPPLOX_SCANNER_H
