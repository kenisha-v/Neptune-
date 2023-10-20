#include "lex.h"
using namespace std;

//returns the type of a token, helps while making an AST.
TokenType getType(char in) {
    if (in == '+' || in == '-' || in == '*' || in == '/' || in == '=') {
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

std::vector<token> tokenize(const std::string& input, int row) {
    vector<token> all_tokens;
    std::istringstream stream(input);
    char ar_op[9] = {'=','+', '-', '*', '/', '(', ')', ' ', '\t'};
    //int row = 1;
    int col = 1;
    std::string temp_str_num = "";
    char in_char;
    bool hasDecimal = false; // Track if current number being processed has a decimal

    //for checkpoint 2 adding identifiers
    std::string temp_identifier = "";
    bool isIdentifier = false; // Track if the current number is a part of an identifier 

    while (stream.get(in_char)) {

        if (in_char == '\n') {
            if (!temp_str_num.empty()){
                all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
                col += temp_str_num.length();
                temp_str_num = "";
                hasDecimal = false;
            }
            //If there's an identifier still in temp_identifier, processing it
            else if (!temp_identifier.empty()){
                all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
                col += temp_identifier.length();
                temp_identifier = "";
                isIdentifier = false;
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
            //If there's an identifier still in temp_identifier, processing it
            else if (!temp_identifier.empty()){
                all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
                col += temp_identifier.length();
                temp_identifier = "";
                isIdentifier = false;
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
            if (isIdentifier) {
                temp_identifier += in_char;
            }
            else {
                if (!(isalpha(stream.peek()) || stream.peek()!='_')) {
                    throw SyntaxError(row, col + temp_str_num.length()+1);
                }
                temp_str_num += in_char;
            }
        }
        else if (in_char == '.') {
            if (!isdigit(stream.peek())) {
                if (!temp_identifier.empty()) {
                    throw SyntaxError(row, col + temp_identifier.length());
                }
                throw SyntaxError(row, col + temp_str_num.length()+1);
            } else if (hasDecimal || temp_str_num.empty()){
                throw SyntaxError(row, col + temp_str_num.length());
            }
            hasDecimal = true;
            temp_str_num += in_char;
        }
        else if (isalpha(in_char) || in_char == '_') {
            temp_identifier += in_char;
            isIdentifier = true;
        }
        else {
            col += temp_str_num.length();
            throw SyntaxError(row, col);
        }
    }
    
    if (!temp_str_num.empty()) {
        all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
    }
    else if (!temp_identifier.empty()){
        all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
        col += temp_identifier.length();
        temp_identifier = "";
        isIdentifier = false;
    }
    
    all_tokens.push_back(getToken(row, col, "END", TokenType::END));
    return all_tokens;
}
