//
// Created by osboxes on 10/16/20.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#if defined __has_include
#  if __has_include (<catch2/catch.hpp>)
#     include <catch2/catch.cpp>
#  elif __has_include("catch.hpp")
#     include "catch.hpp"
#  endif
#endif
