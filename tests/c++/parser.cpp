

#include "Parser.h"
#include "Interpreter.h"
#include "Scanner.h"
#include <iostream>
#if defined __has_include
#if __has_include(<catch2/catch.hpp>)
#include <catch2/catch.hpp>
#elif __has_include("catch.hpp")
#include "catch.hpp"
#endif
#endif

TEST_CASE("Parser") {
  SECTION("Number Literal") {
    // 1
    std::string line = "1";
    Scanner scanner{line};
    auto tokens = scanner.scanTokens();
    for (const auto &token : tokens) {
      // std::cout << "token: " << token << std::endl;
    }
    Parser parser{tokens};
    auto expr = parser.parse();

    // REQUIRE(expr->eval() == Value(1.0));

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(1.0));
  }
  // 1+2
  SECTION("Binary expression") {
    std::string line = "1+2";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    // 1. we need some kind of structure to represent the expression
    // 2. we need something like the Scanner, while !isAtEnd() do things
    // 3. "do things" ==>
    //   a. recognize "operator" ie "+" in 1+2
    //   b. create a "node" that corresponds to #1
    //   c. look ahead
    //
    Parser parser{tokens};
    auto expr = parser.parse();

    // REQUIRE(expr->eval() == Value(3.0));

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(3.0));
  }
  // Homework: 1+2+3
  // Extra credit: 1-2-3 => -4 ,2?
  // 1+2+3
  SECTION("Multiple Binary/PLUS expression") {
    std::string line = "1+2+3";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(6.0));
  }
  // 1-2-3
  SECTION("Multiple Binary/MINUS expression") {
    std::string line = "1-2-3";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(-4.0));
  }
  // 1+2*3
  SECTION("Mix multiply/divsion and addition/minus expression") {
    std::string line = "1+2*3";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(7.0));
  }
  SECTION("Test Equality (double)") {
    std::string line = "1==1";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(true));
  }
  SECTION("Test Inequality (double)") {
    std::string line = "1!=2";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(true));
  }
  SECTION("Test Equality (boolean)") {
    std::string line = "true==true";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(true));
  }
  SECTION("Test Inequality (boolean)") {
    std::string line = "true!=true";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(false));
  }
  SECTION("Test Equality (string)") {
    std::string line = R"("string"=="String")";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(false));
  }
  SECTION("Test Inequality (string)") {
    std::string line = R"("string"!="String")";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(true));
  }
  SECTION("Test Inequality (different type)") {
    std::string line = R"( true !="String")";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(true));
  }

  SECTION("Test comparison (great)") {
    std::string line = "2>1";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(true));
  }
  SECTION("Test comparison (less)") {
    std::string line = "2<1";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(false));
  }
  SECTION("Test comparison (less equal)") {
    std::string line = "2<=1";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(false));
  }

  SECTION("Test comparison (great equal)") {
    std::string line = "2>=2";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(true));
  }

  SECTION("Test unary") {
    std::string line = "2*-1";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(-2.0));
  }

  SECTION("Test multiple unary") {
    std::string line = "---1";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(-1.0));
  }
  SECTION("Parenthesis") {
    std::string line = "2-(1+3)";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(-2.0));
  }
  SECTION("Nested Parenthesis") {
    std::string line = "2-((1+3))";

    Scanner scanner{line};
    auto tokens = scanner.scanTokens();

    Parser parser{tokens};
    auto expr = parser.parse();

    Interpreter interpreter{};
    auto value = interpreter.eval(*expr);
    REQUIRE(value == Value(-2.0));
  }

  SECTION("Error Parser handling") {
        // 1. Parser error
        //    a. create an expression with unbalanced parenthesis
        //    b. parse the expression
        //    c. expect an exception to be thrown
        std::string line = "2-((1+3)";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser{tokens};
        //auto expr = parser.parse();
        REQUIRE_THROWS(parser.parse());
  }
  SECTION("RunTime Error"){
        // 2. Runtime error
        //    a. create binary expression with mistmached to per and ypes
        //    b. parse the epxression
        //    c. expect and exception to be thrown
        // See catch2 documentation for item c.

        std::string line = "1 * true";

        Scanner scanner{line};
        auto tokens = scanner.scanTokens();

        Parser parser{tokens};
        auto expr = parser.parse();

        Interpreter interpreter{};
        //auto value = interpreter.eval();
        REQUIRE_THROWS_AS(interpreter.eval(*expr),std::exception);
  }
}
