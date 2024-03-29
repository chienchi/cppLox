#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include "AST.h"
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
        // primary        → NUMBER | STRING | "true" | "false" | "nil"
        //*                  | "(" expression ")" | IDENTIFIER ;
        if (match(TokenType::IDENTIFIER)) {
            return std::make_unique<Var>(previous());
        }
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

    std::unique_ptr<Expression> assignment() {
        // assignment     -> IDENTIFIER "=" assignment
        //                  equality ;

        // TBD, tricky to translate from Java.
        auto left = equality();

        if (match(TokenType::EQUAL)) {
            auto equals = previous();
            auto value = assignment();
            if (Var *var = dynamic_cast<Var *>(left.get())) {
                auto name = var->name;
                return std::make_unique<Assignment>(name, std::move(value));
            }
            // real l-value expression

            throw parser_error{"Invalid assignment target."};
        }
        return left;
    }

    std::unique_ptr<Expression> expression() {
        // expression     → assignment
        return assignment();
    };

    std::unique_ptr<Stmt> statement() {
        if (match(TokenType::PRINT))
            return printStatement();
        if (match(TokenType::LEFT_BRACE))
            return blockStatement();
        return expressionStatement();
    }

    // TBD: ifStatement() {}

    std::unique_ptr<Stmt> printStatement() {
        auto value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_unique<PrintStmt>(std::move(value));
    }

    std::unique_ptr<Stmt> expressionStatement() {
        auto value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after expression.");
        return std::make_unique<ExprStmt>(std::move(value));
    }

    std::unique_ptr<Stmt> blockStatement() {
        std::vector<std::unique_ptr<Stmt>> statements;
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            statements.push_back(declaration());
        }
        consume(TokenType::RIGHT_BRACE,"Expect '}' after declaration*.");
        return std::make_unique<BlockStmt>(std::move(statements));
    }

    std::unique_ptr<Stmt> varDecl() {
        auto name = consume(TokenType::IDENTIFIER, "variable name expected");
        if (match(TokenType::EQUAL)) {
            auto expr = expression();
            consume(TokenType::SEMICOLON, "Semicolon missing!");
            return std::make_unique<VarDecl>(name, std::move(expr));
        }
        consume(TokenType::SEMICOLON, "Semicolon missing!");
        return std::make_unique<VarDecl>(name);
    }

    std::unique_ptr<Stmt> declaration() {
        if (match(TokenType::VAR)) {
            return varDecl();
        } else {
            return statement();
        }
    }

    std::vector<std::unique_ptr<Stmt>> parse() {
        // Grammars:
        //   expression := term
        //   term := mul_binary ('+-' factor)*
        //   factor := literal ('*/' literal)*
        //   literal  := Number
        /*
         * NEW:
         * program        -> declaration* EOF;
         * declaration    -> varDecl
         *                  | statement
         * varDecl        -> "var" IDENTIFIER ( "=" expression)? ";"
         * statement      -> exprStmt
         *                  | ifStmt
         *                  | printStmt
         *                  | blockStmt;
         * exprStmt       -> expression ";"
         * ifStmt         -> "if" "(" expression ")" statement
         *                   ("else" statement)? ;
         * printStmt      -> "print" expression ";" ;
         * blockStmt      -> "{" declaration* "}";
         *
         * expression     → equality ;
         * equality       → comparison ( ( "!=" | "==" ) comparison )* ;
         * comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
         * term           → factor ( ( "-" | "+" ) factor )* ;
         * factor         → unary ( ( "/" | "*" ) unary )* ;
         * unary          → ( "!" | "-" ) unary
         *                  | primary ;
         * primary        → NUMBER | STRING | "true" | "false" | "nil"
         *                  | "(" expression ")" | IDENTIFIER ;
         */
        std::vector<std::unique_ptr<Stmt>> statements;
        while (!isAtEnd()) {
            statements.push_back(declaration());
        }

        return statements;
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
    template<typename... Ts>
    bool match(Ts... types) {
        // map, apply/mapping a function to elements in a "container"
        // ex. 1. map check() types => check(types[0]), check(types[1]) ...
        // check(types[n]) reduce, apply a binary operator to elements in a
        // "container" and return a single value. ex. reduce || to ex. 1 =>
        // check(types[0]) || check(types[1]) ... ||  check(types[n]) => true/false
        auto visit = [this](auto arg) {
            if (check(arg)) {
                advance();
                return true;
            }
            return false;
        };
        return (... || visit(types));
    }

    bool check(TokenType type);

    Token peek();

    Token advance();

    Token previous();

    Token consume(TokenType type, std::string message);

    bool isAtEnd();

private:
    std::vector<Token> tokens;
    std::size_t current = 0;
};

#endif // CPPLOX_PARSER_H
