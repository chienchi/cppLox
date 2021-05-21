//
// Created by osboxes on 11/13/20.
//

#include <catch2/catch.hpp>
#include "Token.h"
#include <sstream>

TEST_CASE("Literal values")
{
    SECTION("Default construction to contain nil"){
        auto value = Value{};
        REQUIRE(value.index() == 0);
        REQUIRE(std::holds_alternative<std::monostate>(value));
    }
    SECTION("Construction"){
        auto bool_value = Value{true};
        REQUIRE(bool_value.index() == 1);
        REQUIRE(std::holds_alternative<bool>(bool_value));

        auto double_value = Value{3.14159};
        REQUIRE(double_value.index() == 2);
        REQUIRE(std::holds_alternative<double>(double_value));

        // std::string x="hello world";
        // auto string_value = Value{x};
        auto string_value = Value{"hello world"};
        REQUIRE(string_value.index() == 3);
        REQUIRE(std::holds_alternative<std::string>(string_value));
    }

    SECTION("Output"){
        SECTION("Nil"){
            auto nil_value=Value{};
            std::ostringstream os;
            os << nil_value;
            REQUIRE(os.str() == "nil");
        }
        SECTION("Boolean"){
            auto bool_value = Value{true};
            std::ostringstream os;
            os << bool_value;
            REQUIRE(os.str() == "true");
        }
        SECTION("Double"){
            auto double_value = Value{1.5};
            std::ostringstream os;
            os << double_value;
            REQUIRE(os.str() == "1.5");
        }
        SECTION("String"){
            auto value = Value{"Hello World"};
            std::ostringstream os;
            os << value;
            REQUIRE(os.str() == "Hello World");
        }
        SECTION("print"){
            //auto value = Value{"print out"};
            //sean::outstream << value;
        }
    }
}
