#ifndef LEX_H
#define LEX_H

#include <exception>
#include <vector>
#include <string>
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
        message = "Syntax error on line " + std::to_string(row) + " column " + std::to_string(col) + ".";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

//implementing function definitions in header instead of source:
    // reason there was a linker error otherwise,
    // when compiled with both files, duplication of main fn was an error.

//returns the type of a token, helps while making an AST.
TokenType getType(char in) {
    if (in == '+' || in == '-' || in == '*' || in == '/') {
        return TokenType::OPERATOR;
    } else if (in == '(') {
        return TokenType::LEFT_PAREN;
    } else if (in == ')') {
        return TokenType::RIGHT_PAREN;
    }
    return TokenType::END;
}

//makes a struct token instance for the given inputs
token getToken(int r, int c, string t, TokenType p) {
    token tk;
    tk.row = r;
    tk.col = c;
    tk.text = t;
    tk.type = p;
    return tk;
}

std::vector<token> tokenize(const std::string& input) {
    vector<token> all_tokens;
    std::istringstream stream(input);
    char ar_op[8] = {'+', '-', '*', '/', '(', ')', ' ', '\t'};
    int row = 1;
    int col = 1;
    std::string temp_str_num = "";
    char in_char;
    bool hasDecimal = false; // Track if current number being processed has a decimal

    while (stream.get(in_char)) {

        if (in_char == '\n') {
            if (not temp_str_num.empty()){
                all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
                col += temp_str_num.length();
                temp_str_num = "";
                hasDecimal = false;
            }
            row++;
            col = 1;
            continue;
        }
        else if (std::find(std::begin(ar_op), std::end(ar_op), in_char) != std::end(ar_op)) {
            // If there's a number accumulated in temp_str_num, processing it first
            if (!temp_str_num.empty()) {
                all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
                col += temp_str_num.length();
                temp_str_num = "";
                hasDecimal = false;
            }
            // Now processing the operator
            if (in_char != ' ' && in_char != '\t') {
                all_tokens.push_back(getToken(row, col, string(1, in_char), getType(in_char)));
                col++;
            } else {
                col++;
            }
        }

        else if (isdigit(in_char)) {
            temp_str_num += in_char;
        }
        else if (in_char == '.') {
            if (!isdigit(stream.peek())) {
                throw SyntaxError(row, col + temp_str_num.length()+1);
            } else if (hasDecimal || temp_str_num.empty()){
                throw SyntaxError(row, col + temp_str_num.length());
            }
            hasDecimal = true;
            temp_str_num += in_char;
        }
        else {
            col += temp_str_num.length();
            throw SyntaxError(row, col);
        }
    }
    
    if (!temp_str_num.empty()) {
        all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
    }
    
    all_tokens.push_back(getToken(row, col, "END", TokenType::END));
    return all_tokens;
}

// This function will "THROW SyntaxError" on encountering invalid characters.
    // Requesting Parser team to try-catch SytaxError while calling tokenize, and elemintate parser execution if error is already caught during lexer.

#endif // LEX_H
