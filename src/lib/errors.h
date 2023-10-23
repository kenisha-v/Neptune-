#ifndef ERRORS_H
#define ERRORS_H

#include <string>
#include <exception>
#include "lex.h" //for token definition

class ParseError : public std::exception {
public:
    int row;
    int col;
    token error_token;
    std::string message;
    
    ParseError(int r, int c, token t){
        row = r;
        col = c;
        error_token = t;
        message = "Unexpected token at line " + std::to_string(row) + " column " + std::to_string(col) + ": " + error_token.text;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class EvaluationError : public std::exception {
public:
    std::string message;
    
    EvaluationError(std::string message){
        this->message = "Runtime error: " + message;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
