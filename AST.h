#ifndef CPPLOX_AST_H
#define CPPLOX_AST_H

#include "ExprVisitor.h"
#include "StmtVisitor.h"
#include "Token.h"
#include "Value.h"
#include <memory>

// Chapter 5 Abstract Syntax Tree
struct Expression {
  // pure virtual function
  virtual void accept(ExprVisitor &) const = 0;

  virtual ~Expression() = default;
};

struct Var : public Expression {
  explicit Var(Token name) : name(std::move(name)) {}

  ~Var() override = default;

  void accept(ExprVisitor &visitor) const { visitor.visit(*this); }

  Token name;
};

struct Literal : public Expression {
  explicit Literal(Value value) : value(std::move(value)) {}

  ~Literal() override = default;

  void accept(ExprVisitor &visitor) const { visitor.visit(*this); }

  Value value;
};

struct Binary : public Expression {
  Binary(std::unique_ptr<Expression> &&left, Token op,
         std::unique_ptr<Expression> &&right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  void accept(ExprVisitor &visitor) const { visitor.visit(*this); }

  std::unique_ptr<Expression> left;
  Token op;
  std::unique_ptr<Expression> right;
};

struct Unary : public Expression {
  Unary(Token op, std::unique_ptr<Expression> &&right)
      : op(std::move(op)), right(std::move(right)) {}

  void accept(ExprVisitor &visitor) const { visitor.visit(*this); }

  Token op;
  std::unique_ptr<Expression> right;
};

struct Assignment : public Expression {
  Assignment(std::unique_ptr<Expression> &&left, Token op,
             std::unique_ptr<Expression> &&right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  void accept(ExprVisitor &visitor) const { visitor.visit(*this); }

  std::unique_ptr<Expression> left;
  Token op;
  std::unique_ptr<Expression> right;
};

struct Stmt {
  virtual void accept(StmtVisitor &) const = 0;

  virtual ~Stmt() = default;
};

struct ExprStmt : public Stmt {
  ExprStmt(std::unique_ptr<Expression> &&value) : value(std::move(value)) {}

  void accept(StmtVisitor &visitor) const override { visitor.visit(*this); }

  std::unique_ptr<Expression> value;
};

struct PrintStmt : public Stmt {
  PrintStmt(std::unique_ptr<Expression> &&value) : value(std::move(value)) {}

  void accept(StmtVisitor &visitor) const override { visitor.visit(*this); }

  std::unique_ptr<Expression> value;
};

struct VarDecl : public Stmt {
  VarDecl(Token name, std::unique_ptr<Expression> &&init)
      : name(name), init(std::move(init)) {}
  VarDecl(Token name) : name(name) {}

  void accept(StmtVisitor &visitor) const override { visitor.visit(*this); }

  Token name;
  std::unique_ptr<Expression> init;
};

#endif // CPPLOX_AST_H
