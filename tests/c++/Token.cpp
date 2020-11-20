//
// Created by osboxes on 11/20/20.
//

#include "catch.hpp"
#include "Token.h"

TEST_CASE("Token"){
    SECTION("Construction"){
        // Not a call to the consturctor, aggregate initialization

        auto token= Token{TokenType::VAR,"var",{},1};

        REQUIRE(token.type == TokenType::VAR);
        REQUIRE(token.lexeme == "var");
        REQUIRE(token.literal == Value{});
        REQUIRE(token.line == 1);

    }

    SECTION("Output"){
        auto token= Token{TokenType::VAR,"var",{"Hello World"},0};
        std::ostringstream os;
        os << token;
        REQUIRE(os.str() == "0 VAR var Hello World");
    }
}
