//
// Created by osboxes on 11/20/20.
//

// "var language= 'lox';" =>
// Token(VAR), Token(ID), Token(EQUAL), Token(STRING), Token(SEMICOLON)
#include "scanner.h"
char Scanner::advance(){
    current++;
    return source[current-1];
}
char Scanner::peek(){
    return isAtEnd()?'\0': source[current];
}

void Scanner::addToken(TokenType type){
    std::string text = source.substr(start,current-start);
    tokens.push_back(Token{type,text,Value{text},line});
}

bool Scanner::match(char expected) {
    if (isAtEnd() or source[current] != expected) {
        return false;
    }
    current++;
    return true;
}

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
        case '!': addToken(
                match('=')?TokenType::BANG_EQUAL:TokenType::BANG
                );
            break;
        case '=': addToken(
                    match('=')?TokenType::EQUAL_EQUAL:TokenType::EQUAL
            );
            break;
        case '<': addToken(
                    match('=')?TokenType::LESS_EQUAL:TokenType::LESS
            );
            break;
        case '>': addToken(
                    match('=')?TokenType::GREATER_EQUAL:TokenType::GREATER
            );
            break;
        case '/': if (match('/')){
                // A comment goes until the end of the line.
                while(peek() != '\n' && !isAtEnd()) {advance();}
            } else {
                addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            //Ignore whitespace
            break;
        case '\n': line++; break;
    }
}

