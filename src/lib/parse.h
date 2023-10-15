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
        int position_n;
        int cnt;
        AST();
        void construct(std::vector<token> tokenized);
        int counter(std::vector<token> tokenized, TokenType type);
        int syntax_error(std::vector<token> tokenized, Node* main);
        void print(Node* main);
    private:
        Node* create(std::vector<token> tokenized, int position = 0);
};


#endif