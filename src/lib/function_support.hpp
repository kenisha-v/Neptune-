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
            delete i;
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
};

class EXP{
public:
    std::string type;
    ASTree*        expression;
    function_call*   function;
    EXP(ASTree* e):        type("expression"), expression(e){}
    EXP(function_call* f):   type("function")  , function(f){}
    ~EXP() {
        delete expression;
        delete function;
    }
};

#endif