#ifndef STREE_HPP
#define STREE_HPP

#include "ASTree.hpp"

class STree;

class SNode {
protected:
    ASTree* expression;
    SNode* next;

public:
    virtual std::string type() { return "Snode";}
    SNode(ASTree* exp, SNode* next);
    virtual ~SNode();
    virtual void evaluate(std::unordered_map<std::string, value_bd>* var_map);
    virtual void print(int tab) {(void)tab;}
};

class ExpressionNode : public SNode {
public:
    std::string type() {return "exp";}
    explicit ExpressionNode(ASTree* exp, SNode* next);
    ~ExpressionNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class WhileNode : public SNode {
protected:
    STree* trueBranch;
public:
    std::string type() {return "while";}
    explicit WhileNode(ASTree* exp, SNode* next, STree* t);
    ~WhileNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class PrintNode : public SNode {
public:
    std::string type() {return "print";}
    explicit PrintNode(ASTree* exp, SNode* next);
    ~PrintNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class IfNode : public SNode {
protected:
    STree* trueBranch;
    STree* falseBranch;
public:
    std::string type() {return "if";}
    explicit IfNode(ASTree* exp, SNode* next, STree* t, STree* f);
    ~IfNode();
    void evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
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
    SNode* get_head();
    void evaluate();
    void print(int tab);
    ~STree();

};

#endif