#ifndef FUNCTION_SUPPORT_HPP
#define FUNCTION_SUPPORT_HPP

#include <string>
#include "ASTree.hpp"


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

        return mycode->evaluate;
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