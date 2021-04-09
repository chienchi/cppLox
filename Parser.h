

#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H
#include <string>
#include "Token.h"
#include <utility>
#include <vector>

// Chapter 5
struct Expression {
    // pure virtual function
    [[nodiscard]] virtual Value eval() const = 0;
};
struct Literal: public Expression {
    explicit Literal(Value value) : value(std::move(value)){}

    [[nodiscard]] Value eval() const override{
        return value;
    }
    Value value;
};

struct Binary: public Expression {
    Binary(Expression* left, Token op, Expression* right): left(left),op(std::move(op)),right(right){}
    [[nodiscard]] Value eval() const override{
        if(op.type == TokenType::PLUS){
           return Value { std::get<double>(left->eval())+ std::get<double>(right->eval()) };
        } else if(op.type == TokenType::MINUS){
           return Value { std::get<double>(left->eval())- std::get<double>(right->eval()) };
        }
    }
    Expression *left;
    Token op;
    Expression *right;
};

// Chapter 6
class Parser {
public:
    explicit Parser(std::vector<Token>  tokens) : tokens(std::move(tokens)){}

    bool isAtEnd(){
        return current >= tokens.size();
    }
    Expression *literal(){
        // "aggregate" initialization
        // "new" dynamic memory allocation pointer
        return new Literal(tokens[current++].literal);
    };
    Expression *parse(){
        // Grammars:
        //   expression := literal | binary
        //   binary := literal + op + literal
        //   literal := Number
        //while(!isAtEnd())
        auto left = literal();

        while (!isAtEnd()){
            auto op = tokens[current++];
            auto right = literal();
            left = new Binary(left,op,right);
        }

        return left;
    };
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
    explicit Interpreter( const Expression *expr) : expr(expr){}

    Value eval(){
        return expr->eval();
    }

private:
    const Expression *expr;
};

#endif //CPPLOX_PARSER_H
