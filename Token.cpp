//
// Created by osboxes on 11/13/20.
//

#include "Token.h"
#include <unordered_map>


// Homework operator<<(os&, const TokenType& type);
std::ostream& operator<<(std::ostream& os, const TokenType& tt) {
    static const std::unordered_map<TokenType,std::string> TokenMap{
            {TokenType::VAR, "VAR"},
            {TokenType::LEFT_PAREN, "LEFT_PAREN"},
            {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
            {TokenType::LEFT_BRACE, "LEFT_BRACE"},
            {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
            {TokenType::COMMA, "COMMA"},
            {TokenType::DOT, "DOT"},
            {TokenType::MINUS, "MINUS"},
            {TokenType::PLUS, "PLUS"},
            {TokenType::SEMICOLON, "SEMICOLON"},
            {TokenType::SLASH, "SLASH"},
            {TokenType::STAR, "STAR"},
            {TokenType::BANG, "BANG"},
            {TokenType::BANG_EQUAL, "BANG_EQUAL"},
            {TokenType::EQUAL, "EQUAL"},
            {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
            {TokenType::GREATER, "GREATER"},
            {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
            {TokenType::LESS, "LESS"},
            {TokenType::LESS_EQUAL, "LESS_EQUAL"},
            {TokenType::IDENTIFIER, "IDENTIFIER"},
            {TokenType::STRING, "STRING"},
            {TokenType::NUMBER, "NUMBER"},
            {TokenType::AND, "AND"},
            {TokenType::CLASS, "CLASS"},
            {TokenType::ELSE, "ELSE"},
            {TokenType::FALSE, "FALSE"},
            {TokenType::FUN, "FUN"},
            {TokenType::FOR, "FOR"},
            {TokenType::IF, "IF"},
            {TokenType::NIL, "NIL"},
            {TokenType::OR, "OR"},
            {TokenType::PRINT, "PRINT"},
            {TokenType::RETURN, "RETURN"},
            {TokenType::SUPER, "SUPER"},
            {TokenType::THIS, "THIS"},
            {TokenType::TRUE, "TRUE"},
            {TokenType::WHILE, "WHILE"},
            {TokenType::eof, "eof"}
    };
    //if (! TokenMap.find(tt)){}
    os << TokenMap.find(tt)->second;
    return os;
}
// Homework: write operator<<(ostream&, const Token&);
std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << std::to_string(t.line) + " " << t.type << " " + t.lexeme + " " << t.literal;
    return os;
}