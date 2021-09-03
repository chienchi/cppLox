#ifndef CPPLOX_AST_H
#define CPPLOX_AST_H

#include "ExprVisitor.h"
#include "Token.h"
#include "value.h"
#include <memory>

// Chapter 5 Abstract Syntax Tree
struct RuntimeError : std::exception {
    RuntimeError(Token op, std::string message) : token(std::move(op)), message(std::move(message)) {}

    std::string get_msg() {
        return message;
    }

    std::string message;
    Token token;
};

struct Expression {
    // pure virtual function
//    [[nodiscard]] virtual Value eval() const = 0;
    virtual void accept(ExprVisitor&) const= 0;
    //virtual void accept(const Interpretor&) = 0;

    virtual ~Expression() = default;
};

struct Literal : public Expression {
    explicit Literal(Value value) : value(std::move(value)) {}

    ~Literal() override = default;

    //[[nodiscard]] Value eval() const override { return value; }
    void accept(ExprVisitor& visitor) const{
        visitor.visit(*this);
    }
    Value value;
};

struct Binary : public Expression {
    Binary(std::unique_ptr<Expression> &&left, Token op,
           std::unique_ptr<Expression> &&right)
            : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    ~Binary() override = default;

    void accept(ExprVisitor& visitor) const{
        visitor.visit(*this);
    }
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;
};

struct Unary : public Expression {
    Unary(Token op, std::unique_ptr<Expression> &&right)
            : op(std::move(op)), right(std::move(right)) {}

    ~Unary() override = default;

    void accept(ExprVisitor& visitor) const{
        visitor.visit(*this);
    }
    Token op;
    std::unique_ptr<Expression> right;
};

#endif // CPPLOX_AST_H
