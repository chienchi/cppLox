//
// Created by osboxes on 3/12/21.
//

#include "Parser.h"
bool Parser::isAtEnd(){
    return current >= tokens.size();
}
bool Parser::match (TokenType type){
    if (check(type)){
        advance();
        return true;
    }
    return false;
}
bool Parser::check (TokenType type){
    if(isAtEnd()) return false;
    return peek().type == type;
}
Token Parser::peek(){
    return tokens[current];
}
Token Parser::advance(){
    if(!isAtEnd()) {current++;}
    return previous();
}
Token Parser::previous(){
    return tokens[current-1];
}