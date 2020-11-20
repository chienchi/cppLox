//
// Created by osboxes on 11/20/20.
//

// "var language = 'lox';" =>
// Token(VAR), Token(ID), Token(EQUAL), Token(STRING), Token(SEMICOLON)

#include "catch.hpp"
#include "Token.h"

TEST_CASE("Scanner"){
    std::string line = "var language = \"lox\";";

    Scanner scanner{line};
    auto token = scanner.scanToken();

    REQUIRE(tokens[0].type == TokenType::VAR);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[2].type == TokenType::EQUAL);
    REQUIRE(tokens[3].type == TokenType::STRING);
    REQUIRE(tokens[4].type == TokenType::SEMICOLON);
}