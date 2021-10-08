#ifndef CPPLOX_STMTVISITOR_H
#define CPPLOX_STMTVISITOR_H

struct Stmt;
struct ExprStmt;
struct PrintStmt;
struct VarDecl;

struct StmtVisitor {
  virtual void visit(const Stmt &) = 0;
  virtual void visit(const ExprStmt &) = 0;
  virtual void visit(const PrintStmt &) = 0;
  virtual void visit(const VarDecl&) = 0;
};

#endif // CPPLOX_STMTVISITOR_H
