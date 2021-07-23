#ifndef CPPLOX_AST_H
#define CPPLOX_AST_H

#include <memory>
#include "Token.h"
#include "value.h"

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
    [[nodiscard]] virtual Value eval() const = 0;

    virtual ~Expression() = default;
};

struct Literal : public Expression {
    explicit Literal(Value value) : value(std::move(value)) {}

    ~Literal() override = default;

    [[nodiscard]] Value eval() const override { return value; }

    Value value;
};

struct Binary : public Expression {
    Binary(std::unique_ptr<Expression> &&left, Token op,
           std::unique_ptr<Expression> &&right)
            : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    ~Binary() override = default;

    void checkNumberOperands(Token t, Value left, Value right) const {
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
        RuntimeError err(t, "Operand must be numbers.");
        throw err;
    };

    [[nodiscard]] Value eval() const override {
        // FIXME: we need to check if the operand types are correct.
        //  checkNumberOperand().
        // Homework: what happen if left and right are of different types?
        if (op.type == TokenType::PLUS) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) +
                         std::get<double>(right->eval())};
        } else if (op.type == TokenType::MINUS) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) -
                         std::get<double>(right->eval())};
        } else if (op.type == TokenType::STAR) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) *
                         std::get<double>(right->eval())};
        } else if (op.type == TokenType::SLASH) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) /
                         std::get<double>(right->eval())};
        } else if (op.type == TokenType::EQUAL_EQUAL) {
            return Value{left->eval() == right->eval()};
        } else if (op.type == TokenType::BANG_EQUAL) {
            return Value{left->eval() != right->eval()};
        } else if (op.type == TokenType::LESS_EQUAL) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) <=
                         std::get<double>(right->eval())};
        } else if (op.type == TokenType::LESS) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) <
                         std::get<double>(right->eval())};
        } else if (op.type == TokenType::GREATER_EQUAL) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) >=
                         std::get<double>(right->eval())};
        } else if (op.type == TokenType::GREATER) {
            checkNumberOperands(op, left->eval(), right->eval());
            return Value{std::get<double>(left->eval()) >
                         std::get<double>(right->eval())};
        } else {
            return Value{};
        }
    }

    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;
};

struct Unary : public Expression {
    Unary(Token op, std::unique_ptr<Expression> &&right)
            : op(std::move(op)), right(std::move(right)) {}

    ~Unary() override = default;

    void checkNumberOperand(Token t, Value operand) const {
        if (std::holds_alternative<double>(operand)) return;
        throw RuntimeError(t, "Operand must be a number.");
    };

    [[nodiscard]] Value eval() const override {
        if (op.type == TokenType::BANG) {
            return Value{!std::get<bool>(right->eval())};
        } else if (op.type == TokenType::MINUS) {
            checkNumberOperand(op, right->eval());
            return Value{-std::get<double>(right->eval())};
        } else {
            return Value{};
        }
    }

    Token op;
    std::unique_ptr<Expression> right;
};

#endif // CPPLOX_AST_H
