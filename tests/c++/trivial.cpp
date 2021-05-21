//
// Created by osboxes on 10/9/20.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#if defined __has_include
#  if __has_include (<catch2/catch.hpp>)
#     include <catch2/catch.cpp>
#  elif __has_include("catch.hpp")
#     include "catch.hpp"
#  endif
#endif
unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}