#ifndef LEX_H
#define LEX_H

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
    VARIABLES,
    C_OPERATOR,
    L_OPERATOR,
    STATEMENT,
    L_CURLY,
    R_CURLY
};

struct token {
    int         row;
    int         col;
    std::string text;
    TokenType   type;
};

TokenType getType(char in);
token getToken(int r, int c, string t, TokenType p);
std::vector<token> tokenize(const std::string& input);


// This function will "THROW SyntaxError" on encountering invalid characters.
// Requesting Parser team to try-catch SytaxError while calling tokenize, and elemintate parser execution if error is already caught during lexer.

#endif // LEX_H
