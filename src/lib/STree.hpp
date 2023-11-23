#ifndef STREE_HPP
#define STREE_HPP

#include "ASTree.hpp"
//#include "function_support.hpp"

class STree;
class FuncNode; 
class EXP;

class SNode {
protected:
    EXP* expression;
    SNode* next;

public:
    virtual std::string type() { return "Snode";} //used in print to determine the type of the node
    SNode(EXP* exp, SNode* next);
    virtual ~SNode();
    virtual value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    virtual void print(int tab) {(void)tab;}
};

class ExpressionNode : public SNode {
public:
    std::string type() {return "exp";}
    explicit ExpressionNode(EXP* exp, SNode* next);
    ~ExpressionNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class WhileNode : public SNode {
protected:
    STree* trueBranch;
public:
    std::string type() {return "while";}
    explicit WhileNode(EXP* exp, SNode* next, STree* t);
    ~WhileNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class PrintNode : public SNode {
public:
    std::string type() {return "print";}
    explicit PrintNode(EXP* exp, SNode* next);
    ~PrintNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class IfNode : public SNode {
protected:
    STree* trueBranch;
    STree* falseBranch;
public:
    std::string type() {return "if";}
    explicit IfNode(EXP* exp, SNode* next, STree* t, STree* f);
    ~IfNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class FuncNode : public SNode {
protected:
    std::string f_name;
public:
    std::vector<std::string> parameters;
    STree* code;
    std::string type() {return "def";}
    explicit FuncNode(SNode* next, STree* code, std::vector<std::string> p, std::string name);
    ~FuncNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
    //void call(std::vector<token> arguments);
};

class ReturnNode : public SNode {
public:
    std::string type() {return "return";}
    explicit ReturnNode(EXP* exp, SNode* next);
    ~ReturnNode();
    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map);
    // value_bd call(std::unordered_map<std::string, value_bd>* var_map);
    void print(int tab);
};

class STree {
    SNode* head = nullptr;
    std::vector<token> block;
    size_t current_token_index = 0;


    token get_current_token()   {return block[current_token_index];}
    void consume_token()        {current_token_index++;}
    SNode* parse_block();

public:
    std::unordered_map<std::string, value_bd>* var_map;

    STree(std::vector<token> tokens, std::unordered_map<std::string, value_bd>* var_map);
    SNode* get_head();
    value_bd evaluate();
    void print(int tab);
    ~STree();

};








struct function_call{
    std::string name;
    std::vector<ASTree*> arguments;
    function_call(std::string n, std::vector<ASTree*> arg): name(n), arguments(arg){}
    ~function_call(){
        for (auto i: arguments){
            if (i!= nullptr){
                delete i;
            }
        }
    }

    void print(){
        std::cout << name << "(";
        for(size_t i=0; i<arguments.size(); i++){
            if(i==arguments.size()-1){
                //std::cout << dynamic_cast<IdentifierNode*>(arguments[0]->head)->name << std::endl;
                std::cout << arguments.at(i)->print_no_endl();
            }else{
                std::cout << arguments.at(i)->print_no_endl() << ", ";
            }
        }
        std::cout << ")";
    }

    value_bd evaluate(std::unordered_map<std::string, value_bd>* var_map){
        if(var_map->find(name) == var_map->end()){
            throw EvaluationError("function not found");
        }
        value_bd func = (*var_map)[name];
        if (func.type_tag != "function"){
            throw EvaluationError("not a function");
        }
        FuncNode* myfunc = func.Function_Node;
        STree* mycode = myfunc->code;
        std::vector<std::string> myparams = myfunc->parameters;
        
        if(myparams.size() != arguments.size()){
            throw EvaluationError("param size doesnt match");
        }
        for(size_t i=0; i<myparams.size(); i++){
            (*mycode->var_map)[myparams[i]] = arguments[i]->evaluate();
        }
        if (mycode){
            return mycode->evaluate();
        } else {
            value_bd null = value_bd();
            return null;
        }
    }
};

class EXP{ 
public:
    std::string type;
    ASTree*        expression;
    function_call*   function;
    std::unordered_map<std::string, value_bd> dummy;
    EXP(ASTree* e):          type("expression"), expression(e),        function(nullptr){}
    EXP(function_call* f):   type("function")  , expression(nullptr),  function(f)      {}
    EXP(std::vector<token> before_func, function_call* f):   type("function_assigner")  , function(f){
        if (before_func.size()!=0) {
            int temp_row = before_func.back().row;
            int temp_col = before_func.back().col;
            if (before_func.back().text != "="){
                throw ParseError(temp_row, temp_col, before_func.back());
            }
            before_func.pop_back();
            {//add end token to end of each expression that is being sent to ASTree
                token end_token{temp_row,temp_col+1,"END",TokenType::END};
                before_func.push_back(end_token);
            }
            expression = new ASTree(before_func, &dummy);
        }
    }
    ~EXP() {
        delete expression;
        delete function;
    }
};


#endif
