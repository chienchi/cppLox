//
// Created by osboxes on 12/18/20.
//

#ifndef CPPLOX_ERROR_H
#define CPPLOX_ERROR_H

#include <iostream>
static bool hasError = false;

static void errorLog(std::size_t line, const std::string &where,
                     const std::string &msg) {
  std::cerr << "[line " << line << "] Error ";
  std::cerr << where << ": " << msg << std::endl;
  hasError = true;
}
/*void error(size_t line, const std::string &msg){
    report(line,"",msg)
}*/

#endif // CPPLOX_ERROR_H
