//
// Created by osboxes on 11/20/20.
//

// "var language = 'lox';" =>
// Token(VAR), Token(ID), Token(EQUAL), Token(STRING), Token(SEMICOLON)

#include "Scanner.h"
#include <iostream>

#if defined __has_include
#if __has_include(<catch2/catch.hpp>)
#include <catch2/catch.hpp>
#elif __has_include("catch.hpp")
#include "catch.hpp"
#endif
#endif

TEST_CASE("Scanner") {
  SECTION("approximations") {
    std::string line = "var language = \"lox\"";

    Scanner scanner{line};
    // auto tokens = scanner.scanTokens();
    auto tokens = scanner.scanTokens();

    // for (auto token : tokens){
    // std::cout << "token: " << token << std::endl;
    // }
    // 3rd order approximation, deal with backslash, escape sequence.
    // (maybe a non-issue, the "native language" might be able to handle
    // automatically.)

    // std::string line = "12345"
    // std::string line = ".234"
  }

  SECTION("scanner Class") {
    std::string line = "var language =  \"lox\"";
    // std::string line = "var language = 01.2e8";
    Scanner scanner{line};
    auto tokens = scanner.scanTokens();
    for (auto token : tokens) {
      std::cout << "token: " << token << std::endl;
      // std::cout << "token: " << token.type== << std::endl;
    }
    REQUIRE(tokens[0].type == TokenType::VAR);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[2].type == TokenType::EQUAL);
    REQUIRE(tokens[3].type == TokenType::STRING);
    //  REQUIRE(tokens[4].type == TokenType::SEMICOLON);
  }
}