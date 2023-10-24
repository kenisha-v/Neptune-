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

class ParseError : public std::exception {
public:
    int row;
    int col;
    token error_token;
    std::string message;
    
    ParseError(int r, int c, token t){
        row = r;
        col = c;
        error_token = t;
        message = "Unexpected token at line " + std::to_string(row) + " column " + std::to_string(col) + ": " + error_token.text;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class EvaluationError : public std::exception {
public:
    std::string message;
    
    EvaluationError(std::string message){
        this->message = "Runtime error: " + message;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
