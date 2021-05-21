

#include <iostream>
#if defined __has_include
#  if __has_include (<catch2/catch.hpp>)
#     include <catch2/catch.cpp>
#  elif __has_include("catch.hpp")
#     include "catch.hpp"
#  endif
#endif
#include "scanner.h"
#include "Parser.h"

TEST_CASE("Parser"){

    SECTION("Number Literal"){
        //1
        std::string line = "1";
        Scanner scanner{line};
        auto tokens = scanner.scanTokens();
        for (const auto& token : tokens){
           // std::cout << "token: " << token << std::endl;
        }
        Parser parser {tokens};
        auto expr = parser.parse();

        //REQUIRE(expr->eval() == Value(1.0));

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(1.0));

    }
    // 1+2
    SECTION("Binary expression"){
        std::string line ="1+2";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        // 1. we need some kind of structure to represent the expression
        // 2. we need something like the Scanner, while !isAtEnd() do things
        // 3. "do things" ==>
        //   a. recognize "operator" ie "+" in 1+2
        //   b. create a "node" that corresponds to #1
        //   c. look ahead
        //
        Parser parser {tokens};
        auto expr = parser.parse();

        //REQUIRE(expr->eval() == Value(3.0));

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(3.0));
    }

    // Homework: 1+2+3
    // Extra credit: 1-2-3 => -4 ,2?

    // 1+2+3
    SECTION("Multiple Binary/PLUS expression"){
        std::string line ="1+2+3";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser {tokens};
        auto expr = parser.parse();

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(6.0));
    }

    // 1-2-3
    SECTION("Multiple Binary/MINUS expression"){
        std::string line ="1-2-3";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser {tokens};
        auto expr = parser.parse();

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(-4.0));
    }

    // 1+2*3
    SECTION("Mix multiply/divsion and addition/minus expression"){
        std::string line ="1+2*3";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser {tokens};
        auto expr = parser.parse();

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(7.0));
    }
    SECTION("Test Equality"){
        std::string line ="1==1";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser {tokens};
        auto expr = parser.parse();

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(true));
    }
    SECTION("Test comparison"){
        std::string line ="2>1";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser {tokens};
        auto expr = parser.parse();

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(true));
    }
    SECTION("Test unary"){
        std::string line ="2+-1";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser {tokens};
        auto expr = parser.parse();

        Interpreter interpreter {std::move(expr)};
        auto value = interpreter.eval();
        REQUIRE(value == Value(1.0));
    }
}
