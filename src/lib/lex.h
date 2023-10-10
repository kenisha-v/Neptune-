#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <vector>
#include <string>

enum class TokenType {
    NUMBER,
    OPERATOR,
    LEFT_PAREN,
    RIGHT_PAREN,
    END
};

struct token {
    int         row;
    int         col;
    std::string text;
    TokenType   type;
};

std::vector<token> tokenize(std::string input);

#endif // LEX_H


