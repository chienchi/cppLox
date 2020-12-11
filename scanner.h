//
// Created by osboxes on 11/20/20.
//

#ifndef CPPLOX_SCANNER_H
#define CPPLOX_SCANNER_H
#include <string>
#include "Token.h"
#include <vector>


class Scanner{
public:
    Scanner(const std::string& s_source): source(s_source){};

private:
    bool isAtEnd(){
        return current >= source.size();
    }
    auto zeroOrder(){
        std::vector<std::string> stokens;
        // 0th approximation, scan from left to right, slice input
        // string every time we see a white space
        // 1. keep a record of current position
        // 2. terminate when current position >= .size()
        // 3. call string.substr(start,count)
        while (!isAtEnd()){
            if (source[current] == ' '){
                stokens.push_back(source.substr(start, current-start));
                start = current;
            }
            current++;  // ToDO: can we remove this line??
        }
        return stokens;
    }
    auto firstOrder(){
        std::vector<std::string> stokens;
        // 1st order approximation, scan from left to right, slice input
        // string everytime we see a non-alphabet (in addition to white space).
        // 1. keep a record of starting and current position
        // 2. terminate when current > .size()
        // 3. call string.substr(start,count)
        // Q: Expected outcome???
        //    ==> "var", "language", "=", ???
        // Q: What is the definition of "non-alphabet", how exactly do we deal with it?
        // A: We need to treat space differently from other non-alphabet
        while (!isAtEnd()) {
            if (isspace(source[current])){
                current++;
                start = current;
            } else if (isalnum(source[current])){
                while(isalnum(source[current])){
                    current++;
                }
                stokens.push_back(source.substr(start, current-start));
                start = current;
            } else {
                if (source[current++] == '='){
                    stokens.push_back(source.substr(start, current-start));
                } else {
                    current++;
                }
            }
        }
        return stokens;
    }
    auto secondOrder(){
        // 2nd order approximation, deal with "" for string
        std::vector<std::string> stokens;
        while (!isAtEnd()) {
            if (source[current] == '"'){
                current++;
                start = current;
                while (current <= source.size() and source[current] != '"') {
                    // move until next to '"' as a string
                    current++;
                }
                if (start != current){
                    stokens.push_back(source.substr(start, current - start));
                    current++; // step into '"'
                    start = current + 1;
                }
            } else {
                if (!std::isalnum(source[current])) {

                    if (std::isalnum(source[current - 1])) {
                        stokens.push_back(source.substr(start, current - start));
                    } else if (!std::isspace(source[current - 1])) {
                        stokens.push_back(source.substr(start - 1, current - start + 1));
                    }
                    start = current + 1;
                } else {
                    if (start > 0 and !std::isalnum(source[current - 1]) and !std::isspace(source[current - 1])) {
                        stokens.push_back(source.substr(start - 1, current - start + 1));
                    }
                }
            }
            current++;
        }
        return stokens;
    }

public:
    auto scanTokens() {
        //while(!isAtEnd()){
        //    start = current;
        //    scanToken();
        //}
        //tokens.push_back(Token{TokenType::eof,"",Value{},line});
        //return tokens;
        return firstOrder();
    }





private:
    const std::string& source;
    std::size_t start = 0;
    std::size_t current = 0;
    std::size_t line;
    std::vector<Token> tokens;

    char advance();
    void scanToken();
    void addToken(TokenType type);
    bool match(char expected);
    char peek();
};

#endif //CPPLOX_SCANNER_H
