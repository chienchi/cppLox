//
// Created by osboxes on 11/20/20.
//

// "var language= 'lox';" =>
// Token(VAR), Token(ID), Token(EQUAL), Token(STRING), Token(SEMICOLON)
#include "scanner.h"
#include <unordered_map>
#include <iostream>
#include <string>


void Scanner::errorLog(const std::string& where, const std::string& msg) {
    std::cerr << "[line " << line + 1 << "] Error ";
    std::cerr << where << ": " << msg << std::endl;
}
bool Scanner::isAtEnd(){
    return current >= source.size();
}
bool Scanner::isDigit(char c){
    return c >= '0' and c<='9';
}
bool Scanner::isAlpha(char c){
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or c =='_';
}
bool Scanner::isAlphaNumeric(char c){
    return isAlpha(c) or isDigit(c);
}
char Scanner::advance(){
    current++;
    return source[current-1];
}
char Scanner::peek(){
    return isAtEnd()?'\0': source[current];
}
char Scanner::peekNext() {
    return (current + 1 >= source.size())? '\0':source[current+1];
}
void Scanner::addToken(TokenType type){
    std::string text = source.substr(start,current-start);
    tokens.push_back(Token{type,text,Value{text},line});
}

void Scanner::addToken(TokenType type,std::string text){
    if (type == TokenType::NUMBER){
        tokens.push_back(Token{type,text,Value{std::stod(text)},line});
    }else{
        tokens.push_back(Token{type,text,Value{text},line});
    }

}
bool Scanner::match(char expected) {
    if (isAtEnd() or source[current] != expected) {
        return false;
    }
    current++;
    return true;
}

void Scanner::string(){
    while(peek() != '"' and !isAtEnd()){
        if(peek() == '\n'){ line++;}
        advance();
    }
    if (isAtEnd()){
        //error
        errorLog("","Unterminated String!");
        return;
    }

    // The closing ".  and lead to current - start -2 in length of string
    advance();

    std::string value = source.substr(start+1, current - start - 2);
    addToken(TokenType::STRING,value);
}

void Scanner::number(){
    while (isDigit(peek())) {advance();}
    // Look for a fractional part.
    if (peek() == '.' and isDigit(peekNext())){
        // Consume the ".'
        advance();

        while (isDigit(peek())){advance();}
    }

    addToken(TokenType::NUMBER,source.substr(start,current-start));
}

static const std::unordered_map<std::string,TokenType> keywords{
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else",TokenType::ELSE},
        {"false",TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
};

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) {advance();}
    std::string text = source.substr(start,current-start);
    auto search = keywords.find(text);
    if (search == keywords.end()) {
        addToken(TokenType::IDENTIFIER);
    }else{
        addToken(search->second);
    }
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
        case '"': string(); break;

        default:
            if (isDigit(c)){
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                // string(std::initializer list<charT> list)
                std::string str{c};
                errorLog(str,"Unexpected Character.");
            }

            break;
    }
}

