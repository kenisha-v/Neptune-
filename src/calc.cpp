#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "lib/lex.h" //to use tokenize, struct token, enum TokenType
#include "lib/errors.h"

//base class for AST nodes
class ASTNode {
protected:
    int line;
    int column;
public:
    ASTNode(int l, int c) : line(l), column(c) {}

    virtual ~ASTNode() = default;

    virtual double evaluate(std::unordered_map<std::string, double>*){
        return 0.0;
    }

    virtual std::string print(){
        return "";
    }
};

class NumberNode : public ASTNode {
public:
    std::string value;
    explicit NumberNode(int line, int column, const std::string& value)
        : ASTNode(line, column), value(value) {}

    double evaluate(std::unordered_map<std::string, double>*){
        return std::stod(value);
    }

    std::string print() {
        return value;
    }
};

class IdentifierNode : public ASTNode {
public:
    std::string name;
    explicit IdentifierNode(int line, int column, const std::string& name) : ASTNode(line, column), name(name){}
    std::string print() {
        return name;
    }
    double evaluate(std::unordered_map<std::string, double>* var_map){
        if ((*var_map).find(name) == (*var_map).end()) {
            throw EvaluationError("unknown identifier " + name);
        }
        return (*var_map)[name];
    }
};

class AssignmentNode : public ASTNode {
public:
    IdentifierNode* id;
    ASTNode* value;
    AssignmentNode(int line, int column, IdentifierNode* id, ASTNode* value) : ASTNode(line, column), id(id), value(value){}
    ~AssignmentNode(){
        delete id;
        delete value;
    }
    double evaluate(std::unordered_map<std::string, double>* var_map){
        double solved_value_right_node = value->evaluate(var_map);
        (*var_map)[id->name] = solved_value_right_node;
        return solved_value_right_node;
    }
    std::string print(){
        return "(" + id->print() + " = " + value->print() + ")";
    }
};

class AdditionNode : public ASTNode {
public:
    ASTNode *left, *right;
    AdditionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~AdditionNode(){
        delete left;
        delete right;
    }
    double evaluate(std::unordered_map<std::string, double>* var_map){
        double ans = (left->evaluate(var_map) + right->evaluate(var_map));
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " + " + right->print() + ")";
    }
};

class SubtractionNode : public ASTNode {
public:
    ASTNode *left, *right;
    SubtractionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~SubtractionNode(){
        delete left;
        delete right;
    }
    double evaluate(std::unordered_map<std::string, double>* var_map){
        double ans = (left->evaluate(var_map) - right->evaluate(var_map));
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " - " + right->print() + ")";
    }
};

class MultiplicationNode : public ASTNode {
public:
    ASTNode *left, *right;
    MultiplicationNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~MultiplicationNode(){
        delete left;
        delete right;
    }
    double evaluate(std::unordered_map<std::string, double>* var_map){
        double ans = (left->evaluate(var_map) * right->evaluate(var_map));
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " * " + right->print() + ")";
    }
};

class DivisionNode : public ASTNode {
public:
    ASTNode *left, *right;
    DivisionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~DivisionNode(){
        delete left;
        delete right;
    }
    double evaluate(std::unordered_map<std::string, double>* var_map) {
        double rightValue = right->evaluate(var_map);
        if (rightValue == 0.0) {
            throw EvaluationError("division by zero.");
        }
        double ans = left->evaluate(var_map) / rightValue;
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " / " + right->print() + ")";
    }
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
    
    ASTree(const std::vector<token>& Tokens, std::unordered_map<std::string, double>* map){
        tokens=Tokens;
        try {
            head = parse_expression();
            if (get_current_token().type != TokenType::END){
                throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            }
        }  catch (const ParseError& e){
            delete head;
            throw e;
        }
        var_map = map;
    }
    double evaluate()                           { return head->evaluate(var_map); }
    void print()                                { std::cout << head->print() << std::endl;}
    ~ASTree()                                   { delete head; }
};

ASTNode* ASTree::parse_expression() {
    try{
        return parse_assignment();
    } catch (const ParseError& e){
        throw e;
    }
}

ASTNode* ASTree::parse_assignment() {
    ASTNode* node = nullptr;
    ASTNode* value = nullptr;
    
    try{
        node = parse_addition_subtraction();
        
        if (get_current_token().type == TokenType::OPERATOR && get_current_token().text == "=") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            
            if (dynamic_cast<IdentifierNode*>(node) == nullptr) {
                throw ParseError(temp_row, temp_col, temp_token);//
            }
            
            value = parse_assignment();
            return new AssignmentNode(temp_row, temp_col, static_cast<IdentifierNode*>(node), value);
        }
        
        return node;
    } catch (const ParseError& e){
        delete node;
        delete value;
        throw e;
    }
}

ASTNode* ASTree::parse_addition_subtraction() {
    ASTNode* node = nullptr;
    try {
        node = parse_multiplication_division();
        
        while (get_current_token().type == TokenType::OPERATOR && (get_current_token().text == "+" || get_current_token().text == "-")) {
            if (get_current_token().text == "+") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new AdditionNode(temp_row, temp_col, node, parse_multiplication_division());
            } else if (get_current_token().text == "-") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new SubtractionNode(temp_row, temp_col, node, parse_multiplication_division());
            }
        }
        
        return node;
    }  catch (const ParseError& e){
        delete node;
        throw e;
    }
}

ASTNode* ASTree::parse_multiplication_division() {
    ASTNode* node = nullptr;
    try{
        node = parse_factor();
        
        while (get_current_token().type == TokenType::OPERATOR && (get_current_token().text == "*" || get_current_token().text == "/")) {
            if (get_current_token().text == "*") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new MultiplicationNode(temp_row, temp_col, node, parse_factor());
            } else if (get_current_token().text == "/") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new DivisionNode(temp_row, temp_col, node, parse_factor());
            }
        }
        
        return node;
    } catch (const ParseError& e){
        delete node;
        throw e;
    }
}

ASTNode* ASTree::parse_factor() {
    try{
        if (get_current_token().type == TokenType::LEFT_PAREN) {
            consume_token();
            ASTNode* node = parse_expression();
            if (get_current_token().type != TokenType::RIGHT_PAREN) {
                delete node;
                throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            }
            consume_token();
            return node;
        } else if (get_current_token().type == TokenType::NUMBER) {
            ASTNode* node = new NumberNode(get_current_token().row, get_current_token().col, get_current_token().text);
            consume_token();
            return node;
        } else if (get_current_token().type == TokenType::VARIABLES) {
            ASTNode* node = new IdentifierNode(get_current_token().row, get_current_token().col, get_current_token().text);
            consume_token();
            return node;
        } else {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            return nullptr; //will be needing for the case of a unmatched right paren
        }
    } catch (const ParseError& e){
        throw e;
    }
}


int main() {
    std::string input;
    std::unordered_map<std::string, double> Variable_Values; 
    std::unordered_map<std::string, double> backup;
    ASTree* curr_tree = nullptr;

    while (true){
        try {
            std::getline(std::cin, input);
            if (std::cin.eof()) {
                break;
            }
            backup = Variable_Values;
            std::vector<token> input_tokens = tokenize(input);
            curr_tree = new ASTree(input_tokens, &Variable_Values);
            curr_tree->print();
            std::cout << curr_tree->evaluate() << std::endl;
        } catch (const SyntaxError& e) {
            std::cout << e.what() << std::endl;
        } catch (const ParseError& e) {
            Variable_Values = backup; //not needed here, but just to be safe.
            std::cout << e.what() << std::endl;
        } catch (const EvaluationError& e) {
            Variable_Values = backup;
            std::cout << e.what() << std::endl;
        }
        delete curr_tree;
        curr_tree = nullptr;
    }
}
