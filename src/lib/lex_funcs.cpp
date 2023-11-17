#include "lex.h"
#include "errors.h"
using namespace std;

//returns the type of a token, helps while making an AST.
TokenType getType(char in) {
    if (in == '+' || in == '-' || in == '*' || in == '/' || in == '=' || in == '%') {
        return TokenType::OPERATOR;
    } else if (in == '(') {
        return TokenType::LEFT_PAREN;
    } else if (in == ')') {
        return TokenType::RIGHT_PAREN;
    } else if (in == '|' || in == '^' || in == '&') {
        return TokenType::L_OPERATOR;
    } else if (in == '{') {
        return TokenType::L_CURLY;
    } else if (in == '}') {
        return TokenType::R_CURLY;
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
    char ar_op[14] = {'+', '-', '*', '/', '(', ')', ' ', '\t', '%', '{','}', '|', '^', '&'};
    char c_op[4] = {'=', '>', '<', '!'};
    int row = 1;
    int col = 1;
    std::string temp_str_num = "";
    std::string c_oper = "";
    char in_char;
    bool hasDecimal = false; // Track if current number being processed has a decimal

    //for checkpoint 2 adding identifiers
    std::string temp_identifier = "";
    bool isIdentifier = false; // Track if the current number is a part of an identifier
    bool isC_oper = false; //Tracker for conditional operators

    while (stream.get(in_char)) {

        if (isC_oper){

            if (c_oper == "!" && in_char != '=') {
                throw SyntaxError(row, col);
            }
            if (c_oper == "=" && in_char != '=') {
                all_tokens.push_back(getToken(row, col, c_oper, getType('=')));
                col++;
                c_oper = "";
                isC_oper = false;
            }
            else if (in_char != '=') {
                all_tokens.push_back(getToken(row, col, c_oper, TokenType::C_OPERATOR));
                col++;
                c_oper = "";
                isC_oper = false;
            }
            else if (in_char == '=') {
                c_oper += in_char;
                all_tokens.push_back(getToken(row, col, c_oper, TokenType::C_OPERATOR));
                col += c_oper.length();
                c_oper = "";
                isC_oper = false;
                continue;
            }
        }
        if (in_char == '\n') {
            if (!temp_str_num.empty()){
                all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
                col += temp_str_num.length();
                temp_str_num = "";
                hasDecimal = false;
            }
            //If there's an identifier still in temp_identifier, processing it
            else if (!temp_identifier.empty()){
                if (temp_identifier == "while" || temp_identifier == "if" || temp_identifier == "else" || temp_identifier == "print") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::STATEMENT));
                } else if (temp_identifier == "true" || temp_identifier == "false") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::BOOLEAN));
                } else {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
                }
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
                if (temp_identifier == "while" || temp_identifier == "if" || temp_identifier == "else" || temp_identifier == "print") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::STATEMENT));
                } else if (temp_identifier == "true" || temp_identifier == "false") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::BOOLEAN));
                } else {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
                }
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
            //To check if the number is a part of the variable or if it is a digit on its own
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
            //Checking if decimal point is valid or an error
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
        else if (std::find(std::begin(c_op), std::end(c_op), in_char) != std::end(c_op)) {
            // If there's a number accumulated in temp_str_num, processing it first
            if (!temp_str_num.empty()) {
                all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
                col += temp_str_num.length();
                temp_str_num = "";
                hasDecimal = false;
            }
            //If there's an identifier still in temp_identifier, processing it
            else if (!temp_identifier.empty()){
                if (temp_identifier == "while" || temp_identifier == "if" || temp_identifier == "else" || temp_identifier == "print") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::STATEMENT));
                } else if (temp_identifier == "true" || temp_identifier == "false") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::BOOLEAN));
                } else {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
                }
                col += temp_identifier.length();
                temp_identifier = "";
                isIdentifier = false;
            }
            // Now working with the  conditional operators
            c_oper += in_char;
            isC_oper = true;
            continue;
        }
        else if (in_char == ','){
            if (!temp_str_num.empty()) {
                all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
                col += temp_str_num.length();
                temp_str_num = "";
                hasDecimal = false;
            }
            //If there's an identifier still in temp_identifier, processing it
            else if (!temp_identifier.empty()){
                if (temp_identifier == "while" || temp_identifier == "if" || temp_identifier == "else" || temp_identifier == "print") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::STATEMENT));
                } else if (temp_identifier == "true" || temp_identifier == "false") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::BOOLEAN));
                } else {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
                }
                col += temp_identifier.length();
                temp_identifier = "";
                isIdentifier = false;
            }
            all_tokens.push_back(getToken(row, col, string(1, in_char), TokenType::COMMA));
            ++col;
                
        }
        else if (in_char == ';'){
            if (!temp_str_num.empty()) {
                all_tokens.push_back(getToken(row, col, temp_str_num, TokenType::NUMBER));
                col += temp_str_num.length();
                temp_str_num = "";
                hasDecimal = false;
            }
            //If there's an identifier still in temp_identifier, processing it
            else if (!temp_identifier.empty()){
                if (temp_identifier == "while" || temp_identifier == "if" || temp_identifier == "else" || temp_identifier == "print") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::STATEMENT));
                } else if (temp_identifier == "true" || temp_identifier == "false") {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::BOOLEAN));
                } else {
                    all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
                }
                col += temp_identifier.length();
                temp_identifier = "";
                isIdentifier = false;
            }
            all_tokens.push_back(getToken(row, col, string(1, in_char), TokenType::SEMI_COLON));
            ++col;
                
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
        if (temp_identifier == "while" || temp_identifier == "if" || temp_identifier == "else" || temp_identifier == "print") {
            all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::STATEMENT));
        } else if (temp_identifier == "true" || temp_identifier == "false") {
            all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::BOOLEAN));
        } else {
            all_tokens.push_back(getToken(row, col, temp_identifier, TokenType::VARIABLES));
        }
        col += temp_identifier.length();
        temp_identifier = "";
        isIdentifier = false;
    }

    if (isC_oper){
        if (c_oper == "!") {
            throw SyntaxError(row, col);
        }
        if (c_oper == "=") {
            all_tokens.push_back(getToken(row, col, c_oper, getType('=')));
            col++;
            c_oper = "";
            isC_oper = false;
        }
        else {
            all_tokens.push_back(getToken(row, col, c_oper, TokenType::C_OPERATOR));
            col += c_oper.length();
            c_oper = "";
            isC_oper = false;
        }
    }
    
    all_tokens.push_back(getToken(row, col, "END", TokenType::END));
    return all_tokens;
}
