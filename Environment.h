//
// Created by ollie on 10/8/21.
//

#ifndef CPPLOX_ENVIRONMENT_H
#define CPPLOX_ENVIRONMENT_H

#include "Token.h"
#include "Value.h"
#include "error.h"
#include <unordered_map>

class Environment {
public:
    Environment() {}

    explicit Environment(std::shared_ptr<Environment> environment) : enclosing(environment) {}

    void define(const std::string &name, Value value) { values[name] = value; }

    void assign(Token name, Value value) {
        auto iter = values.find(name.lexeme);
        if (iter != values.end()) {
            values[name.lexeme] = value;
        } else if (enclosing) {
            enclosing->assign(name, value);
        } else {
            throw RuntimeError(name, "Undefined variable: " + name.lexeme + ".");
        }
    }

    Value get(Token name) {
        auto iter = values.find(name.lexeme);
        if (iter != values.end()) {
            return values[name.lexeme];
        }
        if (enclosing)
            return enclosing->get(name);

        throw RuntimeError(name,
                           "Undefined variable: " + name.lexeme + ".");
    }


private:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> enclosing;
};

#endif // CPPLOX_ENVIRONMENT_H
