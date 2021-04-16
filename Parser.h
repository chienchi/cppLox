

#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H
#include <string>
#include "Token.h"
#include <utility>
#include <vector>
#include <memory>

// Chapter 5
struct Expression {
    // pure virtual function
    [[nodiscard]] virtual Value eval() const = 0;
    virtual ~Expression() = default;
};

struct Literal: public Expression {
    explicit Literal(Value value) : value(std::move(value)){}
    ~Literal() override = default;
    [[nodiscard]] Value eval() const override{
        return value;
    }
    Value value;
};

struct Binary: public Expression {
    Binary(std::shared_ptr<Expression> left, Token op,std::shared_ptr<Expression> right): left(std::move(left)), op(std::move(op)), right(std::move(right)){}
    ~Binary() override = default;
    [[nodiscard]] Value eval() const override{
        if(op.type == TokenType::PLUS){
           return Value { std::get<double>(left->eval()) + std::get<double>(right->eval()) };
        } else if(op.type == TokenType::MINUS){
           return Value { std::get<double>(left->eval()) - std::get<double>(right->eval()) };
        } else if(op.type == TokenType::STAR){
            return Value { std::get<double>(left->eval()) * std::get<double>(right->eval()) };
        } else if(op.type == TokenType::SLASH){
            return Value { std::get<double>(left->eval()) / std::get<double>(right->eval()) };
        }
    }
    std::shared_ptr<Expression> left;
    Token op;
    std::shared_ptr<Expression> right;
};

// Chapter 6
class Parser {
public:
    explicit Parser(std::vector<Token>  tokens) : tokens(std::move(tokens)){}

    bool isAtEnd(){
        return current >= tokens.size();
    }
    std::shared_ptr<Expression> literal(){
        // "aggregate" initialization
        // "new" dynamic memory allocation pointer
        return std::shared_ptr<Expression>(new Literal(tokens[current++].literal));
    };
    std::shared_ptr<Expression> binary_right_recursion(){
        auto left = literal();
        if (isAtEnd()){
            return left;
        }else{
            // Right recursion
            //   binary := literal op binary (right recursion)
            //   1 -2 -3   => (1 - (2 -3))
            auto op = tokens[current++];
            auto right = binary();   //literal();
            return std::shared_ptr<Expression>(new Binary(left, op, right)); // return type  ==> Binary
        }
    }
    std::shared_ptr<Expression> binary(){
        //   binary := literal + (op + literal)*
        auto left = literal();

        while(!isAtEnd()){
            auto op = tokens[current++];
            auto right = literal();
            left = std::shared_ptr<Expression>(new Binary(left, op, right));
        }
        return left;
    }
    std::shared_ptr<Expression> parse(){
        // Grammars:
        //   expression := literal | binary
        //   binary := binary op literal (left recursion, infinite recursion)
        //   1 -2 - 3  => ((1-2)-3)
        //   literal := Number

        return binary();
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
    explicit Interpreter( const std::shared_ptr<Expression> expr) : expr(expr){}

    Value eval(){
        Value value = expr->eval();
        //delete expr; // Problem only the first level, doesn't not get deeper.
        return value;
    }

private:
    const std::shared_ptr<Expression> expr;
};

#endif //CPPLOX_PARSER_H
