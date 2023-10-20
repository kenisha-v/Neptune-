#ifndef LEX_H
#define LEX_H

#include <exception>
#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

enum class TokenType {
    NUMBER,
    OPERATOR,
    LEFT_PAREN,
    RIGHT_PAREN,
    END,
    VARIABLES
};

struct token {
    int         row;
    int         col;
    std::string text;
    TokenType   type;
};

class SyntaxError : public std::exception {
public:
    int row;
    int col;
    std::string message;
    
    SyntaxError(int r, int c){
        row = r;
        col = c;
        message = "Syntax error on line " + std::to_string(row) + " column " + std::to_string(col) + ".";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

TokenType getType(char in);
token getToken(int r, int c, string t, TokenType p);
std::vector<token> tokenize(const std::string& input, int row, int col);


// This function will "THROW SyntaxError" on encountering invalid characters.
    // Requesting Parser team to try-catch SytaxError while calling tokenize, and elemintate parser execution if error is already caught during lexer.

#endif // LEX_H
