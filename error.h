//
// Created by osboxes on 12/18/20.
//

#ifndef CPPLOX_ERROR_H
#define CPPLOX_ERROR_H

#include <iostream>

enum class hasError{ YES , NO };

class ErrorLog{
public:
    ErrorLog(std::string& i_where, std::string& i_msg): where(i_where), message(i_msg){};
private:
    void report(const int line, const std::string& here, const std::string& msg){
        std::cerr << "[line " << line << "] Error";
        std::cerr << here << ": " << msg << std::endl;
        status = hasError::YES;
    }

public:
    void error(const int line, const std::string& msg){
        report(line,"",msg);
    }
    auto getStatus(){
        return status;
    }
private:
    std::string message;
    std::string where;
    hasError status = hasError::NO;
};


#endif //CPPLOX_ERROR_H
