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
    virtual void accept(const ExprVisitor&) const= 0;
    //virtual void accept(const Interpretor&) = 0;

    virtual ~Expression() = default;
};

struct Literal : public Expression {
    explicit Literal(Value value) : value(std::move(value)) {}

    ~Literal() override = default;

    //[[nodiscard]] Value eval() const override { return value; }
    void accept(const ExprVisitor& visitor) const{
        visitor.visit(*this);
    }
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
        Value rightV = right->eval();
        Value leftV = left->eval();
        if (op.type == TokenType::PLUS) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) +
                         std::get<double>(rightV)};
        } else if (op.type == TokenType::MINUS) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) -
                         std::get<double>(rightV)};
        } else if (op.type == TokenType::STAR) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) *
                         std::get<double>(rightV)};
        } else if (op.type == TokenType::SLASH) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) /
                         std::get<double>(rightV)};
        } else if (op.type == TokenType::EQUAL_EQUAL) {
            return Value{leftV == rightV};
        } else if (op.type == TokenType::BANG_EQUAL) {
            return Value{leftV != rightV};
        } else if (op.type == TokenType::LESS_EQUAL) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) <=
                         std::get<double>(rightV)};
        } else if (op.type == TokenType::LESS) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) <
                         std::get<double>(rightV)};
        } else if (op.type == TokenType::GREATER_EQUAL) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) >=
                         std::get<double>(rightV)};
        } else if (op.type == TokenType::GREATER) {
            checkNumberOperands(op, leftV, rightV);
            return Value{std::get<double>(leftV) >
                         std::get<double>(rightV)};
        } else {
            return Value{};
        }
    }
    void accept(const ExprVisitor& visitor){
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

    void accept(const ExprVisitor& visitor){
        visitor.visit(*this);
    }
    Token op;
    std::unique_ptr<Expression> right;
};

#endif // CPPLOX_AST_H
