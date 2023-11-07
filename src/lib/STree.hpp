#ifndef STREE_HPP
#define STREE_HPP

#include "ASTree.hpp"

class STree;

class SNode {
protected:
    ASTree* expression;
    SNode* next;

public:
    SNode(ASTree* exp, SNode* next);
    virtual ~SNode();
    virtual void evaluate(std::unordered_map<std::string, value_bd>* var_map);
};

class ExpressionNode : public SNode {
public:
    explicit ExpressionNode(ASTree* exp, SNode* next);
    ~ExpressionNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
};

class WhileNode : public SNode {
protected:
    STree* trueBranch;
public:
    explicit WhileNode(ASTree* exp, SNode* next, STree* t);
    ~WhileNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
};

class PrintNode : public SNode {
public:
    explicit PrintNode(ASTree* exp, SNode* next);
    ~PrintNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
};

class IfNode : public SNode {
protected:
    STree* trueBranch;
    STree* falseBranch;
public:
    explicit IfNode(ASTree* exp, SNode* next, STree* t, STree* f);
    ~IfNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
};

class STree {
    SNode* head = nullptr;
    std::vector<token> block;
    size_t current_token_index = 0;
    std::unordered_map<std::string, value_bd>* var_map;

    token get_current_token()   {return block[current_token_index];}
    void consume_token()        {current_token_index++;}
    //get_current_line() return vector of tokens
    //if first token is a statement call appropriate statement node constructor
    //else send new vector to ASTree
    SNode* parse_block();

public:
    STree(std::vector<token> tokens, std::unordered_map<std::string, value_bd>* var_map);
    void evaluate();
    ~STree();

};

#endif