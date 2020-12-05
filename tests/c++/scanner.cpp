//
// Created by osboxes on 11/20/20.
//

// "var language = 'lox';" =>
// Token(VAR), Token(ID), Token(EQUAL), Token(STRING), Token(SEMICOLON)

#include <iostream>
#include "catch.hpp"
#include "scanner.h"

TEST_CASE("Scanner"){
    SECTION("approximations") {
        std::string line = "var language = \"lox\";";

        // 0th approximation, scan from left to right, slice input
        // string every time we see a white space
        // 1. keep a record of current position
        // 2. terminate when current position >= .size()
        // 3. call string.substr(start,count)
        std::cout << "input: " << line << std::endl;
#if 0
        int current = 0, start = 0;
        while (current <= line.size()){
            if (line[current] == ' '){
                std::cout << "token: " << line.substr(start, current-start) << std::endl;
                start = current;
            }
            current++;  // ToDO: can we remove this line??
        }
#endif
        // 1st order approximation, scan from left to right, slice input
        // string everytime we see a non-alphabet (in addition to white space).
        // 1. keep a record of starting and current position
        // 2. terminate when current > .size()
        // 3. call string.substr(start,count)
        // Q: Expected outcome???
        //    ==> "var", "language", "=", ???
        // Q: What is the definition of "non-alphabet", how exactly do we deal with it?
        // A: We need to treat space differently from other non-alphabet
#if 0
        int current = 0, start = 0;
        while (current <= line.size()) {
            if (!std::isalnum(line[current])) {
                if (std::isalnum(line[current-1])){
                    std::cout << "token: " << line.substr(start, current - start) << std::endl;
                }
                else if (!std::isspace(line[current-1]) ){
                    std::cout << "token: " << line.substr(start-1, current - start+1) << std::endl;
                }
                start = current + 1;
            }else{
                if (start > 0 and !std::isalnum(line[current-1]) and !std::isspace(line[current-1])){
                   std::cout << "token: " << line.substr(start-1, current - start+1) << std::endl;
                }
            }
            current++;
        }
#endif
        // 2nd order approximation, deal with "" for string
        int current = 0, start = 0;
        while (current <= line.size()) {
            if (line[current] == '"'){
                current++;
                start = current;
                while (current <= line.size() and line[current] != '"') {
                    // move to next '"' as a string
                    current++;
                }
                if (start != current){
                    std::cout << "token: " << line.substr(start, current - start) << std::endl;
                    current++; // step into '"'
                    start = current + 1;
                }
            } else {
                if (!std::isalnum(line[current])) {

                    if (std::isalnum(line[current - 1])) {
                        std::cout << "token: " << line.substr(start, current - start) << std::endl;
                    } else if (!std::isspace(line[current - 1])) {
                        std::cout << "token: " << " " << line.substr(start - 1, current - start + 1) << std::endl;
                    }
                    start = current + 1;
                } else {
                    if (start > 0 and !std::isalnum(line[current - 1]) and !std::isspace(line[current - 1])) {
                        std::cout << "token3: " << line.substr(start - 1, current - start + 1) << std::endl;
                    }
                }
            }
            current++;
        }
        // 3rd order approximation, deal with backslash, escape sequence.
        // (maybe a non-issue, the "native language" might be able to handle automatically.)

        // std::string line = "12345"
        // std::string line = ".234"
    }

    SECTION("scanner Class") {
        std::string line = "var language = \"lox\";";

        //Scanner scanner{line};
        //auto token = scanner.scanTokens();

        /*REQUIRE(tokens[0].type == TokenType::VAR);
        REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[2].type == TokenType::EQUAL);
        REQUIRE(tokens[3].type == TokenType::STRING);
        REQUIRE(tokens[4].type == TokenType::SEMICOLON);*/
    }
}