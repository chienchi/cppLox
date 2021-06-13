//
// Created by osboxes on 10/16/20.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#if defined __has_include
#if __has_include(<catch2/catch.hpp>)
#include <catch2/catch.hpp>
#elif __has_include("catch.hpp")
#include "catch.hpp"
#endif
#endif

//void test_fun() {}

TEST_CASE("Reading from a source file"){

  // TODO
  // 1. Add CppLox.cpp and main()
  // 2. Add class Lox and runFile()
  // 3. Turn this into runFile(). with run() just comparing string
  // 4. Add runPrompt() which reads from std::cin and calls run();
  // 5. Error and exception handling
  // 6. Token and TokenType
  //
  std::ifstream s("../../../Lox/hello.lox");
  REQUIRE(s.is_open() == true);

  // Read from 'source' into a std::string
  std::stringstream content;
  content << s.rdbuf();

  // Check if the read string is the same as expected.
  std::string expected{"// our first Lox program\n"
                       "print \"Hello World!\";\n"};

  REQUIRE(content.str() == expected);

  // no need to say s.close(), it is automatically closed.
}
