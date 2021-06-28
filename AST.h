#ifndef CPPLOX_AST_H
#define CPPLOX_AST_H

#include <memory>

#include "Token.h"
#include "value.h"

// Chapter 5 Abstract Syntax Tree
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

  [[nodiscard]] Value eval() const override {
    if (op.type == TokenType::PLUS) {
      return Value{std::get<double>(left->eval()) +
                   std::get<double>(right->eval())};
    } else if (op.type == TokenType::MINUS) {
      return Value{std::get<double>(left->eval()) -
                   std::get<double>(right->eval())};
    } else if (op.type == TokenType::STAR) {
      return Value{std::get<double>(left->eval()) *
                   std::get<double>(right->eval())};
    } else if (op.type == TokenType::SLASH) {
      return Value{std::get<double>(left->eval()) /
                   std::get<double>(right->eval())};
    } else if (op.type == TokenType::EQUAL_EQUAL) {
      // Homework: what happen if left and right are of different types?
      return Value{left->eval() == right->eval()};
    } else if (op.type == TokenType::BANG_EQUAL) {
      return Value{left->eval() != right->eval()};
    } else if (op.type == TokenType::LESS_EQUAL) {
      return Value{std::get<double>(left->eval()) <=
                   std::get<double>(right->eval())};
    } else if (op.type == TokenType::LESS) {
      return Value{std::get<double>(left->eval()) <
                   std::get<double>(right->eval())};
    } else if (op.type == TokenType::GREATER_EQUAL) {
      return Value{std::get<double>(left->eval()) >=
                   std::get<double>(right->eval())};
    } else if (op.type == TokenType::GREATER) {
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

  [[nodiscard]] Value eval() const override {
    if (op.type == TokenType::BANG) {
      return Value{!std::get<bool>(right->eval())};
    } else if (op.type == TokenType::MINUS) {
      return Value{-std::get<double>(right->eval())};
    } else {
      return Value{};
    }
  }

  Token op;
  std::unique_ptr<Expression> right;
};

#endif // CPPLOX_AST_H
