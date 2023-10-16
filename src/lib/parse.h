#ifndef PARSE_H
#define PARSE_H

#include "lex.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    std::string text;
    int position;
    TokenType type;
    Node* l_child;
    Node* r_child;
    Node();
    Node(std::string text, int position, TokenType type, Node* l_child = nullptr, Node* r_child = nullptr);
};

class AST{
    public:
        Node* main;
        std::string infix;
        AST();
        void construct(std::vector<token> tokenized);
        int counter(std::vector<token> tokenized, TokenType type);
        int syntax_error(std::vector<token> tokenized, Node* main);
        void print(Node* main);
        double evaluate(Node* main);
    private:
        int next_position;
        Node* create(std::vector<token> tokenized, int position = 0);
};

// class ParseError : public std::exception {
// public:
//     int row;
//     int col;
//     std::string message;
    
//     ParseError(int r, int c){
//         row = r;
//         col = c;
//         message = "Unexpected token at line " + std::to_string(row) + ", column " + std::to_string(col) + ".";
//     }

//     const char* what() const noexcept override {
//         return message.c_str();
//     }
// };

#endif