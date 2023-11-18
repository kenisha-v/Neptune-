#include "ASTree.hpp"

//----------------------

ASTNode::ASTNode(int l, int c) : line(l), column(c) {}
ASTNode::~ASTNode() = default;

value_bd ASTNode::evaluate(std::unordered_map<std::string, value_bd>*){
        return value_bd("bool",0.0); //never used
    }

std::string ASTNode::print(){
        return "";
    }

//----------------------

NumberNode::NumberNode(int line, int column, const std::string& value)
        : ASTNode(line, column), value(value) {}

value_bd NumberNode::evaluate(std::unordered_map<std::string, value_bd>*){
    return value_bd("double",std::stod(value));
    }

std::string NumberNode::print() {
        std::stringstream ss;
        ss << std::stod(value);
        std::string precision_val = ss.str();
        return precision_val;
    }
//----------------------

BooleanNode::BooleanNode(int line, int column, const std::string& value)
        : ASTNode(line, column), value(value) {}

value_bd BooleanNode::evaluate(std::unordered_map<std::string, value_bd>*){
    double double_val = -1;
    if(value == "false") {
        double_val = 0;
    } else if(value == "true") {
        double_val = 1;
    }
    return value_bd("bool", double_val);
    }

std::string BooleanNode::print() {
        return value;
    }

//----------------------

IdentifierNode::IdentifierNode(int line, int column, const std::string& name) : ASTNode(line, column), name(name){}

std::string IdentifierNode::print() {
    return name;
}

value_bd IdentifierNode::evaluate(std::unordered_map<std::string, value_bd>* var_map){
    if ((*var_map).find(name) == (*var_map).end()) {
        throw EvaluationError("unknown identifier " + name);
    }
    return (*var_map)[name];
}

//----------------------

AssignmentNode::AssignmentNode(int line, int column, ASTNode* id, ASTNode* value) : ASTNode(line, column), id(id), value(value){}

AssignmentNode::~AssignmentNode(){
        delete id;
        delete value;
}

value_bd AssignmentNode::evaluate(std::unordered_map<std::string, value_bd>* var_map){
        IdentifierNode* id_node = dynamic_cast<IdentifierNode*>(id);
        if (id_node == nullptr) {
            throw EvaluationError("invalid assignee.");
        }
        value_bd solved_value_right_node = value->evaluate(var_map);
        (*var_map)[id_node->name] = solved_value_right_node;
        delete id_node;
        return solved_value_right_node;
}

std::string AssignmentNode::print(){
        // if (id == nullptr) {
        //     throw EvaluationError("invalid assignee.");
        // }
        return "(" + id->print() + " = " + value->print() + ")";
}

//----------------------

AdditionNode::AdditionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}

AdditionNode::~AdditionNode(){
        delete left;
        delete right;
    }

value_bd AdditionNode::evaluate(std::unordered_map<std::string, value_bd>* var_map){
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("double",(left->evaluate(var_map).Double + right->evaluate(var_map).Double));
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

value_bd SubtractionNode::evaluate(std::unordered_map<std::string, value_bd>* var_map){
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("double",(left->evaluate(var_map).Double - right->evaluate(var_map).Double));
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
    
value_bd MultiplicationNode::evaluate(std::unordered_map<std::string, value_bd>* var_map){
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("double",(left->evaluate(var_map).Double * right->evaluate(var_map).Double));
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
    
value_bd DivisionNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        if (right->evaluate(var_map).Double == 0.0) {
            throw EvaluationError("division by zero.");
        }
        return value_bd("double",(left->evaluate(var_map).Double / right->evaluate(var_map).Double));
    }
    
std::string DivisionNode::print(){
        return "(" + left->print() + " / " + right->print() + ")";
    }

//----------------------

ModuloNode::ModuloNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
ModuloNode::~ModuloNode(){
        delete left;
        delete right;
    }
    
value_bd ModuloNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        if (right->evaluate(var_map).Double == 0.0) {
            throw EvaluationError("division by zero.");
        }
        double ans = std::fmod(left->evaluate(var_map).Double, right->evaluate(var_map).Double);
        return value_bd("double", ans);

    }
    
std::string ModuloNode::print(){
        return "(" + left->print() + " % " + right->print() + ")";
    }

//----------------------

LessNode::LessNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
LessNode::~LessNode(){
        delete left;
        delete right;
    }
    
value_bd LessNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("bool", left->evaluate(var_map).Double < right->evaluate(var_map).Double);
    }
    
std::string LessNode::print(){
        return "(" + left->print() + " < " + right->print() + ")";
    }

//----------------------

LessEqualNode::LessEqualNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
LessEqualNode::~LessEqualNode(){
        delete left;
        delete right;
    }
    
value_bd LessEqualNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("bool", left->evaluate(var_map).Double <= right->evaluate(var_map).Double);
    }
    
std::string LessEqualNode::print(){
        return "(" + left->print() + " <= " + right->print() + ")";
    }

//----------------------

MoreNode::MoreNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
MoreNode::~MoreNode(){
        delete left;
        delete right;
    }
    
value_bd MoreNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("bool", left->evaluate(var_map).Double > right->evaluate(var_map).Double);
    }
    
std::string MoreNode::print(){
        return "(" + left->print() + " > " + right->print() + ")";
    }

//----------------------

MoreEqualNode::MoreEqualNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
MoreEqualNode::~MoreEqualNode(){
        delete left;
        delete right;
    }
    
value_bd MoreEqualNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag == "bool" || right->evaluate(var_map).type_tag == "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("bool", left->evaluate(var_map).Double >= right->evaluate(var_map).Double);
    }
    
std::string MoreEqualNode::print(){
        return "(" + left->print() + " >= " + right->print() + ")";
    }

//----------------------

EqualNode::EqualNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
EqualNode::~EqualNode(){
        delete left;
        delete right;
    }
    
value_bd EqualNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if((left->evaluate(var_map).type_tag == "bool" && right->evaluate(var_map).type_tag != "bool") || (left->evaluate(var_map).type_tag != "bool" && right->evaluate(var_map).type_tag == "bool")) {
            throw EvaluationError("invalid operand type.");
        }
        if(left->evaluate(var_map).type_tag == "bool") {
            return value_bd("bool", left->evaluate(var_map).Bool == right->evaluate(var_map).Bool);
        } else {
            return value_bd("bool", left->evaluate(var_map).Double == right->evaluate(var_map).Double);
        }
    }
    
std::string EqualNode::print(){
        return "(" + left->print() + " == " + right->print() + ")";
    }

//----------------------

NotEqualNode::NotEqualNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
NotEqualNode::~NotEqualNode(){
        delete left;
        delete right;
    }
    
value_bd NotEqualNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if((left->evaluate(var_map).type_tag == "bool" && right->evaluate(var_map).type_tag != "bool") || (left->evaluate(var_map).type_tag != "bool" && right->evaluate(var_map).type_tag == "bool")) {
            throw EvaluationError("invalid operand type.");
        }
        if(left->evaluate(var_map).type_tag == "bool") {
            return value_bd("bool", left->evaluate(var_map).Bool != right->evaluate(var_map).Bool);
        } else {
            return value_bd("bool", left->evaluate(var_map).Double != right->evaluate(var_map).Double);
        }
    }
    
std::string NotEqualNode::print(){
        return "(" + left->print() + " != " + right->print() + ")";
    }

//----------------------

LandNode::LandNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
LandNode::~LandNode(){
        delete left;
        delete right;
    }
    
value_bd LandNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag != "bool" || right->evaluate(var_map).type_tag != "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("bool", left->evaluate(var_map).Bool && right->evaluate(var_map).Bool);
    }
    
std::string LandNode::print(){
        return "(" + left->print() + " & " + right->print() + ")";
    }

//----------------------

LxorNode::LxorNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
LxorNode::~LxorNode(){
        delete left;
        delete right;
    }
    
value_bd LxorNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag != "bool" || right->evaluate(var_map).type_tag != "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("bool", ((!left->evaluate(var_map).Bool) && right->evaluate(var_map).Bool) || (left->evaluate(var_map).Bool && (!right->evaluate(var_map).Bool)));
    }
    
std::string LxorNode::print(){
        return "(" + left->print() + " ^ " + right->print() + ")";
    }

//----------------------

LorNode::LorNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    
LorNode::~LorNode(){
        delete left;
        delete right;
    }
    
value_bd LorNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if(left->evaluate(var_map).type_tag != "bool" || right->evaluate(var_map).type_tag != "bool"){
            throw EvaluationError("invalid operand type.");
        }
        return value_bd("bool", left->evaluate(var_map).Bool || right->evaluate(var_map).Bool);
    }
    
std::string LorNode::print(){
        return "(" + left->print() + " | " + right->print() + ")";
    }

//----------------------


//ASTree Public Function Definitions
ASTree::ASTree(const std::vector<token>& Tokens, std::unordered_map<std::string, value_bd>* map){
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

value_bd ASTree::evaluate(){
    return head->evaluate(var_map);
}

void ASTree::print(){
    std::cout << head->print() << std::endl;
}

std::string ASTree::print_no_endl(){
    //std::cout << head->print();
    std::string input;
    input += head->print();
    //std::cout << input;
    return input;
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
        node = parse_Lor();

        if (get_current_token().type == TokenType::OPERATOR && get_current_token().text == "=") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            
            value = parse_assignment();
            return new AssignmentNode(temp_row, temp_col, node, value);
        }
        
        return node;
    } catch (const ParseError& e){
        delete node;
        delete value;

        throw e;
    }
}

ASTNode* ASTree::parse_Lor() {
    ASTNode* node = nullptr;
    ASTNode* value = nullptr;
    
    try{
        node = parse_Lxor();
        
        while (get_current_token().type == TokenType::L_OPERATOR && get_current_token().text == "|") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            node = new LorNode(temp_row, temp_col, node, parse_Lxor());
        }
        
        return node;
    } catch (const ParseError& e){
        delete node;
        delete value;
        throw e;
    }
}

ASTNode* ASTree::parse_Lxor() {
    ASTNode* node = nullptr;
    ASTNode* value = nullptr;
    
    try{
        node = parse_Land();
        
        while (get_current_token().type == TokenType::L_OPERATOR && get_current_token().text == "^") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            node = new LxorNode(temp_row, temp_col, node, parse_Land());
        }
        
        return node;
    } catch (const ParseError& e){
        delete node;
        delete value;
        throw e;
    }
}

ASTNode* ASTree::parse_Land() {
    ASTNode* node = nullptr;
    ASTNode* value = nullptr;
    
    try{
        node = parse_equality();
        
        while (get_current_token().type == TokenType::L_OPERATOR && get_current_token().text == "&") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            node = new LandNode(temp_row, temp_col, node, parse_equality());
        }
        
        return node;
    } catch (const ParseError& e){
        delete node;
        delete value;
        throw e;
    }
}

ASTNode* ASTree::parse_equality() {
    ASTNode* node = nullptr;
    try {
        node = parse_compare();

        while(get_current_token().type == TokenType::C_OPERATOR && (get_current_token().text == "==" || get_current_token().text == "!=")) {
            if (get_current_token().text == "==") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new EqualNode(temp_row, temp_col, node, parse_compare());
            } else if (get_current_token().text == "!=") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new NotEqualNode(temp_row, temp_col, node, parse_compare());
            }
        }

        return node;
    }  catch (const ParseError& e){
        delete node;
        throw e;
    }
}

ASTNode* ASTree::parse_compare() {
    ASTNode* node = nullptr;
    try {
        node = parse_addition_subtraction();

        while(get_current_token().type == TokenType::C_OPERATOR && (get_current_token().text == ">" || get_current_token().text == ">=" || get_current_token().text == "<" || get_current_token().text == "<=")) {
            if (get_current_token().text == ">") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new MoreNode(temp_row, temp_col, node, parse_addition_subtraction());
            } else if (get_current_token().text == ">=") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new MoreEqualNode(temp_row, temp_col, node, parse_addition_subtraction());
            } else if (get_current_token().text == "<") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new LessNode(temp_row, temp_col, node, parse_addition_subtraction());
            } else if (get_current_token().text == "<=") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new LessEqualNode(temp_row, temp_col, node, parse_addition_subtraction());
            }
        }

        return node;
    }  catch (const ParseError& e){
        delete node;
        throw e;
    }
}

ASTNode* ASTree::parse_addition_subtraction() {
    ASTNode* node = nullptr;
    try {
        node = parse_multiplication_division_modulo();
        
        while (get_current_token().type == TokenType::OPERATOR && (get_current_token().text == "+" || get_current_token().text == "-")) {
            if (get_current_token().text == "+") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new AdditionNode(temp_row, temp_col, node, parse_multiplication_division_modulo());
            } else if (get_current_token().text == "-") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new SubtractionNode(temp_row, temp_col, node, parse_multiplication_division_modulo());
            }
        }
        
        return node;
    }  catch (const ParseError& e){
        delete node;
        throw e;
    }
}

ASTNode* ASTree::parse_multiplication_division_modulo() {
    ASTNode* node = nullptr;
    try{
        node = parse_factor();
        
        while (get_current_token().type == TokenType::OPERATOR && (get_current_token().text == "*" || get_current_token().text == "/" || get_current_token().text == "%")) {
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
            } else if (get_current_token().text == "%") {
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                std::string temp_text   = get_current_token().text;
                consume_token();
                node = new ModuloNode(temp_row, temp_col, node, parse_factor());
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
        } else if (get_current_token().type == TokenType::BOOLEAN) {
            ASTNode* node = new BooleanNode(get_current_token().row, get_current_token().col, get_current_token().text);
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

