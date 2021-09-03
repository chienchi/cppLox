#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include "AST.h"
#include "ExprVisitor.h"
#include "Token.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

// Chapter 6
class Parser {
public:
  explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

  std::unique_ptr<Expression> literal() {
    // "aggregate" initialization
    // "new" dynamic memory allocation pointer
    return std::make_unique<Literal>(tokens[current++].literal);
  };

  std::unique_ptr<Expression> binary_right_recursion() {
    auto left = literal();
    if (isAtEnd()) {
      return left;
    } else {
      // Right recursion
      //   binary := literal op binary (right recursion)
      //   1 -2 -3   => (1 - (2 -3))
      auto op = tokens[current++];
      auto right = factor(); // literal();
      return std::make_unique<Binary>(
          std::move(left), op, std::move(right)); // return type  ==> Binary
    }
  }
  struct parser_error : std::exception {
    parser_error(std::string string);
    std::string message;
  };
  std::unique_ptr<Expression> primary() {
    if (match(TokenType::LEFT_PAREN)) {
      auto expr = expression();
      if (match(TokenType::RIGHT_PAREN)) {
        return expr;
      } // else?? FIXME: throw Parse Error, consume()?
      else {
        throw parser_error{"Unbalanced parenthesis!"};
      }
    }
    return literal();
  }

  std::unique_ptr<Expression> unary() {
    //   unary  :=  ( "!" | "-" ) unary | primary ;

    if (match(TokenType::BANG, TokenType::MINUS)) { // don't need while loop
      auto op = previous();
      auto right = unary(); // this is recursive itself
      return std::make_unique<Unary>(op, std::move(right));
    }

    return primary();
  }

  std::unique_ptr<Expression> factor() {
    //   factor := unary ('*/' unary)*
    auto left = unary();

    while (match(TokenType::STAR, TokenType::SLASH)) {
      auto op = previous();
      auto right = unary();
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return left;
  }

  std::unique_ptr<Expression> term() {
    //   term := factor ('+-' factor)*
    auto left = factor();

    while (match(TokenType::PLUS, TokenType::MINUS)) {
      auto op = previous();
      auto right = factor();
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return left;
  }

  std::unique_ptr<Expression> comparison() {
    // comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
    auto left = term();
    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL,
                 TokenType::LESS_EQUAL, TokenType::LESS)) {
      auto op = previous();
      auto right = term();
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return left;
  }

  std::unique_ptr<Expression> equality() {
    // equality       → comparison ( ( "!=" | "==" ) comparison )* ;
    auto left = comparison();

    while (match(TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL)) {
      auto op = previous();
      auto right = comparison();
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return left;
  }

  std::unique_ptr<Expression> expression() {
    // expression     → equality ;
    return equality();
  };

  std::unique_ptr<Expression> parse() {
    // Grammars:
    //   expression := term
    //   term := mul_binary ('+-' factor)*
    //   factor := literal ('*/' literal)*
    //   literal  := Number
    /*
     * expression     → equality ;
     * equality       → comparison ( ( "!=" | "==" ) comparison )* ;
     * comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
     * term           → factor ( ( "-" | "+" ) factor )* ;
     * factor         → unary ( ( "/" | "*" ) unary )* ;
     * unary          → ( "!" | "-" ) unary
     *                  | primary ;
     * primary        → NUMBER | STRING | "true" | "false" | "nil"
     *                  | "(" expression ")" ;
     */
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
  // template in .h
  template <typename... Ts> bool match(Ts... types) {
    return (... || check(types));
    // map, apply/mapping a function to elements in a "container"
    // ex. 1. map check() types => check(types[0]), check(types[1]) ...
    // check(types[n]) reduce, apply a binary operator to elements in a
    // "container" and return a single value. ex. reduce || to ex. 1 =>
    // check(types[0]) || check(types[1]) ... ||  check(types[n]) => true/false
  }
  bool check(TokenType type);
  Token peek();
  Token advance();
  Token previous();
  bool isAtEnd();

private:
  std::vector<Token> tokens;
  std::size_t current = 0;
};

// Chapter 7
class Interpreter : public ExprVisitor {
public:
  //explicit Interpreter(std::unique_ptr<Expression> &&expr)
  //    : expr(std::move(expr)) {}

  Value eval(const Expression& expr) {
      visit(expr);
      return result;
  }
  void visit(const Expression& expr){
      expr.accept(*this);
  }
  void visit(const Literal& exp){
      result= exp.value;
  }
  void visit(const Unary& exp){
      exp.right->accept(*this);//this.result will be changed.
      if (exp.op.type == TokenType::BANG) {
          result=Value{!std::get<bool>(result)};
      } else if (exp.op.type == TokenType::MINUS) {
          checkNumberOperand(exp.op, result);
          result=Value{-std::get<double>(result)};
      } else {
          result=Value{}; //TODO: throw exception
      }
  }
  void visit(const Binary& exp){
      exp.left->accept(*this);
      Value leftV = result;
      exp.right->accept(*this);
      Value rightV = result;
      if (exp.op.type == TokenType::PLUS) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) +
          std::get<double>(rightV)};
      } else if (exp.op.type == TokenType::MINUS) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) -
          std::get<double>(rightV)};
      } else if (exp.op.type == TokenType::STAR) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) *
          std::get<double>(rightV)};
      } else if (exp.op.type == TokenType::SLASH) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) /
          std::get<double>(rightV)};
      } else if (exp.op.type == TokenType::EQUAL_EQUAL) {
          result = Value{leftV == rightV};
      } else if (exp.op.type == TokenType::BANG_EQUAL) {
          result = Value{leftV != rightV};
      } else if (exp.op.type == TokenType::LESS_EQUAL) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) <=
          std::get<double>(rightV)};
      } else if (exp.op.type == TokenType::LESS) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) <
          std::get<double>(rightV)};
      } else if (exp.op.type == TokenType::GREATER_EQUAL) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) >=
          std::get<double>(rightV)};
      } else if (exp.op.type == TokenType::GREATER) {
          checkNumberOperands(exp.op, leftV, rightV);
          result = Value{std::get<double>(leftV) >
          std::get<double>(rightV)};
      } else {
          result = Value{};
      }
  }
  void checkNumberOperands(Token t, Value left, Value right) const {
      if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
      RuntimeError err(t, "Operand must be numbers.");
      throw err;
  };
  void checkNumberOperand(Token t, Value operand) const {
      if (std::holds_alternative<double>(operand)) return;
      throw RuntimeError(t, "Operand must be a number.");
  };
  // implement all of the visit() virtual functions.

private:
    Value result;
  //std::unique_ptr<Expression> expr;
};

#endif // CPPLOX_PARSER_H
