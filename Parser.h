//
// Created by osboxes on 3/12/21.
//

#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H
#include <string>
#include "Token.h"
#include <vector>
// Chapter 5
struct Literal{

    Literal(Value value) : value(value){}

    Value value;
};

// Chapter 6
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens){}

    auto parse(){
        //while(!isAtEnd())
        return Literal(tokens[0].literal);
    }

private:
    std::vector<Token> tokens;
};

// Chapter 7
class Interpreter{
public:
    explicit Interpreter(const Literal& expr) : expr(expr){}

    Value eval(){
        return expr.value;
    }

private:
    Literal expr;
};

#endif //CPPLOX_PARSER_H
