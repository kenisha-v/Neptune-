#ifndef VALUE_BD_HPP
#define VALUE_BD_HPP

#include <string>

class FuncNode;

struct value_bd{
    std::string type_tag;
    bool Bool;
    double Double;
    FuncNode* Function_Node;

    value_bd() : type_tag("null"), Bool(false), Double(0.0), Function_Node(nullptr) {}
    value_bd(std::string tag, double value): type_tag(tag), Function_Node(nullptr){
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
    value_bd(FuncNode* func_ptr): type_tag("function"), Function_Node(func_ptr){}

};

#endif