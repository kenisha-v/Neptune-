#ifndef LEX_H
#define LEX_H

#include <vector>
#include <string>
#include <exception>

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

class SyntaxError : public std::exception {
public:
    int row;
    int col;
    std::string message;
    
    SyntaxError(int r, int c){
        row = r;
        col = c;
        message = "Syntax error at line " + std::to_string(row) + ", column " + std::to_string(col) + ".";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};


std::vector<token> tokenize(std::string input);
// This function will "THROW SyntaxError" on encountering invalid characters.
// Requesting Parser team to try-catch SytaxError while calling tokenize, and elemintate parser execution if error is already caught during lexer.

#endif // LEX_H
