//
// Created by ollie on 9/3/21.
//

#ifndef CPPLOX_EXPRVISITOR_H
#define CPPLOX_EXPRVISITOR_H
struct Expression;
struct Literal;
struct Unary;
struct Binary;

struct ExprVisitor
{
  virtual void visit(const Expression&) = 0;
  virtual void visit(const Literal&) = 0;
  virtual void visit(const Unary&) = 0;
  virtual void visit(const Binary&) = 0;
};
#endif // CPPLOX_EXPRVISITOR_H
