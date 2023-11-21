#ifndef ASTREE_HPP
#define ASTREE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <cmath> //fmod in modulo

#include "lex.h" //token, TokenType defined here
#include "errors.h"//error classes defined here

struct value_bd{
    std::string type_tag;
    bool Bool;
    double Double;
    std::vector<value_bd>* array;

    value_bd() : type_tag(""), Bool(false), Double(0.0), array(new std::vector<value_bd>) {}
    value_bd(std::string tag, double value): type_tag(tag){
        if (tag == "bool"){
            if (value==0){
                Bool = false;
            } else {
                Bool = true;
            }
        } else {
            Double = value;
        }
    }
    value_bd(std::string tag, std::vector<value_bd>* array): type_tag(tag), array(array){}
};

// Base class for AST nodes
class ASTNode {
protected:
    int line;
    int column;

public:
    ASTNode(int l, int c);
    virtual ~ASTNode();

    virtual value_bd evaluate(std::unordered_map<std::string, value_bd>*);
    virtual std::string print();
};

class NumberNode : public ASTNode {
public:
    std::string value;
    explicit NumberNode(int line, int column, const std::string& value);
    value_bd evaluate(std::unordered_map<std::string, value_bd>*);
    std::string print();
};

class BooleanNode : public ASTNode {
public:
    std::string value;
    explicit BooleanNode(int line, int column, const std::string& value);
    value_bd evaluate(std::unordered_map<std::string, value_bd>*);
    std::string print();
};

class IdentifierNode : public ASTNode {
public:
    std::string name;
    explicit IdentifierNode(int line, int column, const std::string& name);
    std::string print();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
};

class AssignmentNode : public ASTNode {
public:
    ASTNode* id;
    ASTNode* value;
    AssignmentNode(int line, int column, ASTNode* id, ASTNode* value);
    ~AssignmentNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class AdditionNode : public ASTNode {
public:
    ASTNode *left, *right;
    AdditionNode(int line, int column, ASTNode* left, ASTNode* right);
    ~AdditionNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class SubtractionNode : public ASTNode {
public:
    ASTNode *left, *right;
    SubtractionNode(int line, int column, ASTNode* left, ASTNode* right);
    ~SubtractionNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class MultiplicationNode : public ASTNode {
public:
    ASTNode *left, *right;
    MultiplicationNode(int line, int column, ASTNode* left, ASTNode* right);
    ~MultiplicationNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class DivisionNode : public ASTNode {
public:
    ASTNode *left, *right;
    DivisionNode(int line, int column, ASTNode* left, ASTNode* right);
    ~DivisionNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class ModuloNode : public ASTNode {
public:
    ASTNode *left, *right;
    ModuloNode(int line, int column, ASTNode* left, ASTNode* right);
    ~ModuloNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class LessNode : public ASTNode {
public:
    ASTNode *left, *right;
    LessNode(int line, int column, ASTNode* left, ASTNode* right);
    ~LessNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class LessEqualNode : public ASTNode {
public:
    ASTNode *left, *right;
    LessEqualNode(int line, int column, ASTNode* left, ASTNode* right);
    ~LessEqualNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class MoreNode : public ASTNode {
public:
    ASTNode *left, *right;
    MoreNode(int line, int column, ASTNode* left, ASTNode* right);
    ~MoreNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class MoreEqualNode : public ASTNode {
public:
    ASTNode *left, *right;
    MoreEqualNode(int line, int column, ASTNode* left, ASTNode* right);
    ~MoreEqualNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class EqualNode : public ASTNode {
public:
    ASTNode *left, *right;
    EqualNode(int line, int column, ASTNode* left, ASTNode* right);
    ~EqualNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class NotEqualNode : public ASTNode {
public:
    ASTNode *left, *right;
    NotEqualNode(int line, int column, ASTNode* left, ASTNode* right);
    ~NotEqualNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class LandNode : public ASTNode {
public:
    ASTNode *left, *right;
    LandNode(int line, int column, ASTNode* left, ASTNode* right);
    ~LandNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class LxorNode : public ASTNode {
public:
    ASTNode *left, *right;
    LxorNode(int line, int column, ASTNode* left, ASTNode* right);
    ~LxorNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class LorNode : public ASTNode {
public:
    ASTNode *left, *right;
    LorNode(int line, int column, ASTNode* left, ASTNode* right);
    ~LorNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};

class ArrayNode : public ASTNode {
public:
    ASTNode *left, *right;
    ASTNode* node;
    std::vector<value_bd>* array;
    int position;
    ArrayNode(int line, int column, std::vector<value_bd>* array);
    ArrayNode(int line, int column, ASTNode* node, int position);
    ~ArrayNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    std::string print();
};


class ASTree {
    std::vector<token> tokens;
    size_t current_token_index = 0;
    ASTNode* head = nullptr;
    std::unordered_map<std::string, value_bd>* var_map;

    token get_current_token()   {return tokens[current_token_index];}
    void consume_token()        {current_token_index++;}
    
    ASTNode* parse_expression();
    ASTNode* parse_assignment();
    ASTNode* parse_Lor();
    ASTNode* parse_Lxor();
    ASTNode* parse_Land();
    ASTNode* parse_equality();
    ASTNode* parse_compare();
    ASTNode* parse_addition_subtraction();
    ASTNode* parse_multiplication_division_modulo();
    ASTNode* parse_factor();

public:
    
    ASTree(const std::vector<token>& Tokens, std::unordered_map<std::string, value_bd>* map);
    value_bd evaluate();
    void print();
    std::string print_no_endl();
    ~ASTree();
};

#endif // ASTREE_HPP
