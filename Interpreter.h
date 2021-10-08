//
// Created by ollie on 9/10/21.
//

#ifndef CPPLOX_INTERPRETER_H
#define CPPLOX_INTERPRETER_H

#include <iostream>
#include "AST.h"
#include "ExprVisitor.h"
#include "Environment.h"
#include "error.h"

// Chapter 7
class Interpreter : public ExprVisitor, public StmtVisitor {
public:
  // TODO: not needed any more ???
  Value eval(const Expression &expr) {
    visit(expr);
    return result;
  }

  void visit(const Expression &expr) { expr.accept(*this); }

  void visit(const Literal &exp) { result = exp.value; }

  void visit(const Unary &exp) {
    exp.right->accept(*this); // this.result will be changed.
    if (exp.op.type == TokenType::BANG) {
      result = Value{!std::get<bool>(result)};
    } else if (exp.op.type == TokenType::MINUS) {
      checkNumberOperand(exp.op, result);
      result = Value{-std::get<double>(result)};
    } else {
      result = Value{}; // TODO: throw exception
    }
  }

  void visit(const Binary &exp) {
    exp.left->accept(*this);
    Value leftV = result;
    exp.right->accept(*this);
    Value rightV = result;
    if (exp.op.type == TokenType::PLUS) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) + std::get<double>(rightV)};
    } else if (exp.op.type == TokenType::MINUS) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) - std::get<double>(rightV)};
    } else if (exp.op.type == TokenType::STAR) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) * std::get<double>(rightV)};
    } else if (exp.op.type == TokenType::SLASH) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) / std::get<double>(rightV)};
    } else if (exp.op.type == TokenType::EQUAL_EQUAL) {
      result = Value{leftV == rightV};
    } else if (exp.op.type == TokenType::BANG_EQUAL) {
      result = Value{leftV != rightV};
    } else if (exp.op.type == TokenType::LESS_EQUAL) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) <= std::get<double>(rightV)};
    } else if (exp.op.type == TokenType::LESS) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) < std::get<double>(rightV)};
    } else if (exp.op.type == TokenType::GREATER_EQUAL) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) >= std::get<double>(rightV)};
    } else if (exp.op.type == TokenType::GREATER) {
      checkNumberOperands(exp.op, leftV, rightV);
      result = Value{std::get<double>(leftV) > std::get<double>(rightV)};
    } else {
      result = Value{};
    }
  }

  void visit(const Var &expr) {
    // TBD
    result = environment.get(expr.name);
  }

  void visit(const Stmt& statement) {
    statement.accept(*this);
  }

  void visit(const ExprStmt& statement) {
    statement.value->accept(*this);
  }

  void visit(const PrintStmt& statement) {
    statement.value->accept(*this);
    std::cout<<result<<std::endl;
  }

  void visit(const VarDecl& statement) {
    //TBD
    Value value;
    if (statement.init){
        value=eval(*statement.init);
    }
    environment.define(statement.name.lexeme, value);
  }


  void interpret(std::vector<std::unique_ptr<Stmt>>& statements) {
    try{
        for (auto& statement : statements){
            execute(statement);
        }
    }catch(RuntimeError err){
        std::cout << "Error: " + err.get_msg() << std::endl;
    }
  }

  void execute(std::unique_ptr<Stmt> &statement) {
      statement->accept(*this);
  }

  Value get_result(){
      return result;
  }
private:
  void checkNumberOperand(Token t, Value operand) const {
    if (std::holds_alternative<double>(operand))
      return;
    throw RuntimeError(t, "Operand must be a number.");
  };

  void checkNumberOperands(Token t, Value left, Value right) const {
    if (std::holds_alternative<double>(left) &&
        std::holds_alternative<double>(right))
      return;
    RuntimeError err(t, "Operand must be numbers.");
    throw err;
  };

  Value result;

  // TODO: Add an instance of Environment
  Environment environment;
};

#endif // CPPLOX_INTERPRETER_H
