//
// Created by osboxes on 3/12/21.
//

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

        //for (auto token:tokens){
        //    std::cout << token << std::endl;
        //}

        Parser parser {tokens};
        auto expr = parser.parse();

        REQUIRE(expr.value == Value(1.0));

        Interpreter interpreter {expr};
        auto value = interpreter.eval();
        REQUIRE(value == Value(1.0));

    }
    // 1+2, 1+2+3
}