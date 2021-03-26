

#include <iostream>
#include "catch.hpp"
#include "scanner.h"
#include "Parser.h"

TEST_CASE("Parser"){

    SECTION("Number Literal"){
        //1
        std::string line = "1";
        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser {tokens};
        auto expr = parser.parse();

        REQUIRE(expr.value == Value(1.0));

        Interpreter interpreter {expr};
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

        REQUIRE(expr.value == Value(3.0));

        Interpreter interpreter {expr};
        auto value = interpreter.eval();
        REQUIRE(value == Value(3.0));
    }

    // 1+2+3
}