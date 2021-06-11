#ifndef CPPLOX_AST_H
#define CPPLOX_AST_H

#include <memory>

// Chapter 5
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
    }
  }
  std::unique_ptr<Expression> left;
  Token op;
  std::unique_ptr<Expression> right;
};

#endif // CPPLOX_AST_H
