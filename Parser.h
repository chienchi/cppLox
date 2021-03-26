

#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H
#include <string>
#include "Token.h"
#include <vector>
// Chapter 5
struct Expression {};
struct Literal: public Expression {

    //Literal(Value value) : value(value){}

    Value value;
};

struct Binary: public Expression {
    Literal left;
    Token op;
    Literal right;
};

// Chapter 6
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens){}

    bool isAtEnd(){
        return current >= tokens.size();
    }
    auto literal(){
        // "aggregate" initialization
        return Literal {tokens[current++].literal};
    }
    auto parse(){
        // Grammars:
        //   expression := literal | binary
        //   binary := literal + op + literal
        //   literal := Number
        //while(!isAtEnd())
        auto left = literal();
        if(isAtEnd()){
            return left;
        }
        auto op = tokens[current++];
        auto right = literal();

        return Binary {left,op,right};
    }
    // is insufficient to see the left or right
    // while (!isAtEnd()){
    //        switch(current_token){
    //           case Literal:
    //                do something
    //                break;
    //           default:
    //                //error
    //        }
    // }


private:
    std::vector<Token> tokens;
    std::size_t current = 0;
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
