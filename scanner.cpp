//
// Created by osboxes on 11/20/20.
//

// "var language= 'lox';" =>
// Token(VAR), Token(ID), Token(EQUAL), Token(STRING), Token(SEMICOLON)
#include "scanner.h"
char Scanner::advance(){
    current++;
    return source[current-1];
};

void Scanner::addToken(TokenType type){
    std::string text = source.substr(start,current-start);
    tokens.push_back(Token{type,text,Value{text},line});
};
void Scanner::scanToken(){
    char c = advance();
    switch(c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
    }
};

