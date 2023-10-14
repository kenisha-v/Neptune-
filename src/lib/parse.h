#ifndef PARSE_H
#define PARSE_H

#include "lex.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    std::string text;
    Node* l_child;
    Node* r_child;
    Node();
    Node(std::string text, Node* l_child = nullptr, Node* r_child = nullptr);
};

class AST{
    public:
        Node* main;
        int position_n;
        std::string expression;
        AST();
        void construct(std::vector<token> tokenized);
        int error(std::string expression);
        void print(Node* main);
    private:
        Node* create(std::vector<token> tokenized, int position = 0);
};


#endif