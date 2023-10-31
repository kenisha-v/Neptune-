#include "ASTree.hpp"

//----------------------

ASTNode::ASTNode(int l, int c) : line(l), column(c) {}
ASTNode::~ASTNode() = default;

double ASTNode::evaluate(std::unordered_map<std::string, double>*){
        return 0.0;
    }

std::string ASTNode::print(){
        return "";
    }

//----------------------

NumberNode::NumberNode(int line, int column, const std::string& value)
        : ASTNode(line, column), value(value) {}

double NumberNode::evaluate(std::unordered_map<std::string, double>*){
        return std::stod(value);
    }

std::string NumberNode::print() {
        return value;
    }

//----------------------

IdentifierNode::IdentifierNode(int line, int column, const std::string& name) : ASTNode(line, column), name(name){}

std::string IdentifierNode::print() {
    return name;
}

double IdentifierNode::evaluate(std::unordered_map<std::string, double>* var_map){
    if ((*var_map).find(name) == (*var_map).end()) {
        throw EvaluationError("unknown identifier " + name);
    }
    return (*var_map)[name];
}

//----------------------

AssignmentNode::AssignmentNode(int line, int column, IdentifierNode* id, ASTNode* value) : ASTNode(line, column), id(id), value(value){}

AssignmentNode::~AssignmentNode(){
        delete id;
        delete value;
}

double AssignmentNode::evaluate(std::unordered_map<std::string, double>* var_map){
        double solved_value_right_node = value->evaluate(var_map);
        (*var_map)[id->name] = solved_value_right_node;
        return solved_value_right_node;
}

std::string AssignmentNode::print(){
        return "(" + id->print() + " = " + value->print() + ")";
}

//----------------------

AdditionNode::AdditionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}

AdditionNode::~AdditionNode(){
        delete left;
        delete right;
    }

double AdditionNode::evaluate(std::unordered_map<std::string, double>* var_map){
        double ans = (left->evaluate(var_map) + right->evaluate(var_map));
        return ans;
    }
    
std::string AdditionNode::print(){
        return "(" + left->print() + " + " + right->print() + ")";
    }

//----------------------

SubtractionNode::SubtractionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}

SubtractionNode::~SubtractionNode(){
        delete left;
        delete right;
    }

double SubtractionNode::evaluate(std::unordered_map<std::string, double>* var_map){
        double ans = (left->evaluate(var_map) - right->evaluate(var_map));
        return ans;
    }

std::string SubtractionNode::print(){
        return "(" + left->print() + " - " + right->print() + ")";
    }

//----------------------

MultiplicationNode::MultiplicationNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
MultiplicationNode::~MultiplicationNode(){
        delete left;
        delete right;
    }
    
double MultiplicationNode::evaluate(std::unordered_map<std::string, double>* var_map){
        double ans = (left->evaluate(var_map) * right->evaluate(var_map));
        return ans;
    }
    
std::string MultiplicationNode::print(){
        return "(" + left->print() + " * " + right->print() + ")";
    }

//----------------------

DivisionNode::DivisionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
DivisionNode::~DivisionNode(){
        delete left;
        delete right;
    }
    
double DivisionNode::evaluate(std::unordered_map<std::string, double>* var_map) {
        double rightValue = right->evaluate(var_map);
        if (rightValue == 0.0) {
            throw EvaluationError("division by zero.");
        }
        double ans = left->evaluate(var_map) / rightValue;
        return ans;
    }
    
std::string DivisionNode::print(){
        return "(" + left->print() + " / " + right->print() + ")";
    }

//----------------------





//ASTree Public Function Definitions
ASTree::ASTree(const std::vector<token>& Tokens, std::unordered_map<std::string, double>* map){
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

ASTree::~ASTree(){
    delete head;
}

double ASTree::evaluate(){
    return head->evaluate(var_map);
}

void ASTree::print(){
    std::cout << head->print() << std::endl;
}


//PARSING FUNCTION DEFINITIONS
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

