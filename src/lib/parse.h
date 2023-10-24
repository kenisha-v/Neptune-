#ifndef PARSE_H
#define PARSE_H

#include "lex.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>


using namespace std;

struct Node {
    std::string text;
    std::vector<Node*> children;
    Node* parent;

    //constructor and destructor
    Node(Node* parent, std::string text = "");
    ~Node();
};

class AST{
    public:
        Node* head;
        std::string infix;
        AST(std::vector<token> tokenized);
        ~AST();
        double evaluate(Node* head);
        void printAST(Node* head);
        std::map<std::string, double> getVariables();
        void updateVariables(std::map<std::string, double> symbolTable);
    private:
        std::map<std::string, double> symbolTable;

};

#endif
