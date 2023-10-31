#ifndef ASTREE_HPP
#define ASTREE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "lex.h" //token, TokenType defined here
#include "errors.h"//error classes defined here

// Base class for AST nodes
class ASTNode {
protected:
    int line;
    int column;

public: 
    ASTNode(int l, int c);
    virtual ~ASTNode();

    virtual double evaluate(std::unordered_map<std::string, double>*);
    virtual std::string print();
};

class NumberNode : public ASTNode {
public:
    std::string value;
    explicit NumberNode(int line, int column, const std::string& value);
    double evaluate(std::unordered_map<std::string, double>*);
    std::string print();
};

class IdentifierNode : public ASTNode {
public:
    std::string name;
    explicit IdentifierNode(int line, int column, const std::string& name);
    std::string print();
    double evaluate(std::unordered_map<std::string, double>* var_map);
};

class AssignmentNode : public ASTNode {
public:
    IdentifierNode* id;
    ASTNode* value;
    AssignmentNode(int line, int column, IdentifierNode* id, ASTNode* value);
    ~AssignmentNode();
    double evaluate(std::unordered_map<std::string, double>* var_map);
    std::string print();
};

class AdditionNode : public ASTNode {
public:
    ASTNode *left, *right;
    AdditionNode(int line, int column, ASTNode* left, ASTNode* right);
    ~AdditionNode();
    double evaluate(std::unordered_map<std::string, double>* var_map);
    std::string print();
};

class SubtractionNode : public ASTNode {
public:
    ASTNode *left, *right;
    SubtractionNode(int line, int column, ASTNode* left, ASTNode* right);
    ~SubtractionNode();
    double evaluate(std::unordered_map<std::string, double>* var_map);
    std::string print();
};

class MultiplicationNode : public ASTNode {
public:
    ASTNode *left, *right;
    MultiplicationNode(int line, int column, ASTNode* left, ASTNode* right);
    ~MultiplicationNode();
    double evaluate(std::unordered_map<std::string, double>* var_map);
    std::string print();
};

class DivisionNode : public ASTNode {
public:
    ASTNode *left, *right;
    DivisionNode(int line, int column, ASTNode* left, ASTNode* right);
    ~DivisionNode();
    double evaluate(std::unordered_map<std::string, double>* var_map);
    std::string print();
};


class ASTree {
    std::vector<token> tokens;
    size_t current_token_index = 0;
    ASTNode* head = nullptr;
    std::unordered_map<std::string, double>* var_map;

    token get_current_token()   {return tokens[current_token_index];}
    void consume_token()        {current_token_index++;}
    
    ASTNode* parse_expression();
    ASTNode* parse_assignment();
    ASTNode* parse_addition_subtraction();
    ASTNode* parse_multiplication_division();
    ASTNode* parse_factor();

public:
    
    ASTree(const std::vector<token>& Tokens, std::unordered_map<std::string, double>* map);
    double evaluate();
    void print();
    ~ASTree();
};

#endif // ASTREE_HPP
