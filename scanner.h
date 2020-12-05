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

    auto scanTokens() {
        while(!isAtEnd()){
            start = current;
            scanToken();
        }
        tokens.push_back(Token{TokenType::eof,"",Value{},line});
        return tokens;
    }
    bool isAtEnd(){
        return current >= source.size();
    }
    void scanToken();
    char advance();
    void addToken(TokenType type);
private:
    const std::string& source;
    std::size_t start = 0;
    std::size_t current = 0;
    std::size_t line;
    std::vector<Token> tokens;
};

#endif //CPPLOX_SCANNER_H
