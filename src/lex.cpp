#include "lib/lex.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

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

token getToken(int r, int c, string t, TokenType p) {
    token tk;
    tk.row = r;
    tk.col = c;
    tk.text = t;
    tk.type = p;
    return tk;
}

void printTokens(const std::vector<token>& tokens) {
    
    for(const token& token : tokens) {
        std::cout << std::setw(4) << std::right << token.row << "   " << std::setw(2) << std::right << token.col << "  " << token.text << std::endl;
    }
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
            if (in_char != ' ' || in_char != '\t') {
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

int main() {
    std::string input;
    char ch;
    
    while (std::cin.get(ch)) {
        input += ch;
    }

    try {
        auto tokens = tokenize(input);
        printTokens(tokens);
    } catch(const SyntaxError& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
