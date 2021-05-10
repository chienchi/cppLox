

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
    Binary(std::unique_ptr<Expression>&& left, Token op, std::unique_ptr<Expression>&& right): 
           left(std::move(left)), op(std::move(op)), right(std::move(right)){}

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
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;
};

// Chapter 6
class Parser {
public:
    explicit Parser(std::vector<Token>  tokens) : tokens(std::move(tokens)){}


    std::unique_ptr<Expression> literal(){
        // "aggregate" initialization
        // "new" dynamic memory allocation pointer
        return std::make_unique<Literal>(tokens[current++].literal);
    };
    std::unique_ptr<Expression> binary_right_recursion(){
        auto left = literal();
        if (isAtEnd()){
            return left;
        }else{
            // Right recursion
            //   binary := literal op binary (right recursion)
            //   1 -2 -3   => (1 - (2 -3))
            auto op = tokens[current++];
            auto right = mul_binary();   //literal();
            return std::make_unique<Binary>(std::move(left), op, std::move(right)); // return type  ==> Binary
        }
    }
    std::unique_ptr<Expression> mul_binary(){
        //   binary := literal + (op + literal)*
        //   mul_binary := literal ('*/' literal)*
        auto left = literal();

        while(match(TokenType::STAR) or match(TokenType::SLASH)){
            auto op = previous();
            auto right = literal();
            left = std::make_unique<Binary>(std::move(left), op, std::move(right));
        }
        return left;
    }

    std::unique_ptr<Expression> add_binary() {
        //   add_binary := mul_binary ('+-' mul_binary)*
        auto left=mul_binary();

        // match()
        // peek()
        // previous() ...

        while(match(TokenType::PLUS) or match(TokenType::MINUS)){
            auto op = previous();
            // Homework: if (??) is needed?
            auto right = mul_binary();
            left = std::make_unique<Binary>(std::move(left), op, std::move(right));
        }
        return left;
    }

    std::unique_ptr<Expression> expression(){
        //   expression := add_binary
        return add_binary();
    };

    std::unique_ptr<Expression> parse(){
        // Grammars:
        //   expression := add_binary
        //   add_binary := mul_binary ('+-' mul_binary)*
        //   mul_binary := literal ('*/' literal)*
        //   literal  := Number

        return expression();
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
    bool match (TokenType type);
    bool check (TokenType type);
    Token peek();
    Token advance();
    Token previous();
    bool isAtEnd();

private:
    std::vector<Token> tokens;
    std::size_t current = 0;
};

// Chapter 7
class Interpreter{
public:
    explicit Interpreter(std::unique_ptr<Expression>&& expr) : expr(std::move(expr)){}

    Value eval(){
        return expr->eval();
    }

private:
    std::unique_ptr<Expression> expr;
};

#endif //CPPLOX_PARSER_H
