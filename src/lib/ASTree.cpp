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
    if (name == "null") {
        return value_bd("null", "null");
    }
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
        if (dynamic_cast<IdentifierNode*>(id) == nullptr && dynamic_cast<ArrayNode*>(id) == nullptr) {
            throw EvaluationError("invalid assignee.");
        }
        if (dynamic_cast<IdentifierNode*>(id) == nullptr) {
            ArrayNode* id_n = static_cast<ArrayNode*>(id);
            value_bd solved_value_right_node = value->evaluate(var_map);
            std::vector<value_bd> array_left = (*var_map)[id_n->name].array;
            std::ostringstream os;
            os << id_n->position.Double;
            size_t pos = std::stoi(os.str());
            std::vector<value_bd> array_changed;
            for (size_t i = 0; i < array_left.size();++i){
                if (i==pos) {
                    array_changed.push_back(solved_value_right_node);
                } else {
                    array_changed.push_back(array_left[i]);
                }
            }
            value_bd lhs = value_bd("array",array_changed);
            (*var_map)[id_n->name] = lhs;
            return solved_value_right_node;
        }
        IdentifierNode* id_n = static_cast<IdentifierNode*>(id);
        value_bd solved_value_right_node = value->evaluate(var_map);
        (*var_map)[id_n->name] = solved_value_right_node;
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
        if(left->evaluate(var_map).type_tag != right->evaluate(var_map).type_tag) {
            return value_bd("bool", false);
        }
        if (left->evaluate(var_map).type_tag == "array" || right->evaluate(var_map).type_tag == "array") {
            if (left->evaluate(var_map).array.size() != right->evaluate(var_map).array.size()) {
                return value_bd("bool", false);
            }
            // for (size_t i = 0; i< )
            return value_bd("bool",true);
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
        if(left->evaluate(var_map).type_tag != right->evaluate(var_map).type_tag) {
            return value_bd("bool", true);
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

ArrayNode::ArrayNode(int line, int column, std::vector<value_bd> array, std::vector<std::string> array_ele, std::string name) : ASTNode(line, column), name(name){
    this->array = {};
    for (size_t i = 0; i< array.size(); ++i ) {
        this->array.push_back(array[i]);
    }
    this->array_ele = {};
    for (size_t i = 0; i< array_ele.size(); ++i ) {
        this->array_ele.push_back(array_ele[i]);
    }
    this->node = nullptr;
    this->func = false;
}

ArrayNode::ArrayNode(int line, int column, ASTNode* node, value_bd position, std::vector<std::string> array_ele, std::string name) : ASTNode(line, column), node(node), array_ele(array_ele), position(position), name(name), func(false) {}
    
ArrayNode::ArrayNode(int line, int column, std::vector<value_bd> array): ASTNode(line, column){
    this->array = {};
    for (size_t i = 0; i< array.size(); ++i ) {
        this->array.push_back(array[i]);
    }
    this->node = nullptr;
    this->func = false;
}

ArrayNode::ArrayNode(int line, int column, bool func, value_bd func_output ,std::vector<std::string> array_ele) : ASTNode(line, column), array_ele(array_ele), func_output(func_output), func(func) {}

ArrayNode::~ArrayNode(){
        delete node;
    }
    
value_bd ArrayNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        if (func == true) {
            return func_output;
        }
        if (node != nullptr) {
            value_bd val = (*var_map)[node->print()];
            if (node->print()[0] == '[') {
                val = node->evaluate(var_map);
            }
            if (position.type_tag!="double") {
                throw EvaluationError("index is not a number.");
            }
            if (position.Double < 0 || position.Double >= val.array.size()) {
                throw EvaluationError("index out of bounds. really?");
            }
            return val.array[position.Double];
        }
        return value_bd("array", array, array_ele);
    }
    
std::string ArrayNode::print(){
        std::string array_str;
        if (func) {
            for (size_t i = 0; i< array_ele.size(); ++i ) {
                array_str+= array_ele[i];
            }
            return array_str;
        }
        if (node != nullptr){
            for (size_t i = 0; i< array_ele.size(); ++i ) {
                array_str+= array_ele[i];
            }
            return array_str;
        }
        array_str+="[";
        if (array_ele.size()>0) {
            for (size_t i = 0; i< array_ele.size()-1; ++i ) {
                array_str+= array_ele[i];
                array_str+=", ";
            }
            array_str+= array_ele[array_ele.size()-1];
        }
        array_str+="]";
        return array_str;
    }

std::string ArrayNode::evaluate_print(std::vector<value_bd> arr) {
    std::string str;
    str+="[";
    if (arr.size()>0) {
        for (size_t i = 0; i< arr.size()-1; ++i) {
            std::ostringstream os;
            if(arr[i].type_tag == "bool") {
                os << arr[i].Bool;
                if (os.str()=="1"){
                    str+="true";
                } else {
                    str+="false";
                }
            } else if(arr[i].type_tag == "double") {
                os << arr[i].Double;
                str+=os.str();
            } else if (arr[i].type_tag == "null") {
                str+=arr[i].Null;
            } else {
                ArrayNode* arr_ele = new ArrayNode(0,0, arr[i].array);
                str+=arr_ele->evaluate_print(arr[i].array);
                delete arr_ele;
                arr_ele = nullptr;
            }
            str+=", ";
        }
        std::ostringstream os;
        if(arr[arr.size()-1].type_tag == "bool") {
            os << arr[arr.size()-1].Bool;
            if (os.str()=="1"){
                str+="true";
            } else {
                str+="false";
            }
        } else if(arr[arr.size()-1].type_tag == "double") {
            os << arr[arr.size()-1].Double;
            str+=os.str();
        } else if (arr[arr.size()-1].type_tag == "null") {
            str+=arr[arr.size()-1].Null;
        } else {
            ArrayNode* arr_ele = new ArrayNode(0,0, arr[arr.size()-1].array);
            str+=arr_ele->evaluate_print(arr[arr.size()-1].array);
            delete arr_ele;
            arr_ele = nullptr;
        }
    }
    str+="]";
    return str;
}

//----------------------


//ASTree Public Function Definitions
ASTree::ASTree(const std::vector<token>& Tokens, std::unordered_map<std::string, value_bd>* map){
    tokens=Tokens;
    var_map = map;
    if (var_map == nullptr) {
        var_map = new std::unordered_map<std::string, value_bd>;
    }
    try {
        head = parse_expression();
        if (get_current_token().type != TokenType::END){
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }
    }  catch (const ParseError& e){
        delete head;
        throw e;
    }
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
    std::string input;
    input += head->print();
    return input;
}

value_bd len(value_bd output) {
    int size_array;
    if (output.type_tag == "array") {
        size_array = output.array.size();
    } else {
        size_array = 1;
    }
    return value_bd("double", size_array);
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
        } else if (get_current_token().type == TokenType::L_SQUARE){
            ArrayNode* nod = static_cast<ArrayNode*>(node);
            std::string name = nod->name;
            std::vector<std::string> id_s;
            id_s.push_back(nod->print());
            std::vector<token> id_n;
            if (get_current_token().type == TokenType::L_SQUARE){
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                token temp_token        = get_current_token();
                id_s.push_back(get_current_token().text);
                consume_token();
                value_bd pos;
                while (true) {
                    if (get_current_token().type == TokenType::END) {
                        throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                    } else if (get_current_token().type == TokenType::R_SQUARE) {
                        break;
                    }
                    id_n.push_back(get_current_token());
                    consume_token();
                }
                if (get_current_token().type != TokenType::R_SQUARE) {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
                consume_token();
                id_n.push_back(getToken(get_current_token().row, get_current_token().col, "END", TokenType::END));
                ASTree* expression_tree = new ASTree(id_n, var_map);
                id_s.push_back(expression_tree->print_no_endl());
                id_s.push_back("]");
                pos = expression_tree->evaluate();
                value_bd value_var_map = value_bd("array", (*nod).array);
                if(var_map == nullptr){
                    var_map = new std::unordered_map<std::string, value_bd>;
                }
                (*var_map)[name] = value_var_map;
                delete expression_tree;
                return new ArrayNode(temp_row, temp_col, nod, pos, id_s, name);
            }
        } else if (get_current_token().type == TokenType::LEFT_PAREN) {
            std::vector<std::string> array_ele;
            array_ele.push_back(node->print());
            array_ele.push_back("(");
            std::vector<token> funcs;
            consume_token();
            value_bd output;
            while (true) {
                if (get_current_token().type == TokenType::END) {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
                if (get_current_token().type == TokenType::RIGHT_PAREN) {
                    break;
                }
                else {
                    funcs.push_back(get_current_token());
                }
                consume_token();
            }
            ASTree* nodes = nullptr;
            if (funcs.size()==1) {
                output = (*var_map)[funcs[0].text];
                array_ele.push_back(funcs[0].text);
            } else {
                funcs.push_back(getToken(get_current_token().row, get_current_token().col, "END", TokenType::END));
                nodes = new ASTree(funcs, var_map);
                output = nodes->evaluate();
                array_ele.push_back(nodes->print_no_endl());
            }
            array_ele.push_back(get_current_token().text);
            if (node->print() == "len") {
                    output = len(output);
                    int temp_row            = get_current_token().row;
                    int temp_col            = get_current_token().col;
                    consume_token();
                    delete nodes;
                    return new ArrayNode(temp_row, temp_col, true, output, array_ele);
            }
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            consume_token();
            delete nodes;
            return new ArrayNode(temp_row, temp_col, true, output, array_ele);
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
            std::string name = get_current_token().text;
            ASTNode* node = new IdentifierNode(get_current_token().row, get_current_token().col, get_current_token().text);
            std::vector<std::string> id_s;
            id_s.push_back(node->print());
            std::vector<token> id_n;
            consume_token();
            if (get_current_token().type == TokenType::L_SQUARE){
                int temp_row            = get_current_token().row;
                int temp_col            = get_current_token().col;
                token temp_token        = get_current_token();
                id_s.push_back(get_current_token().text);
                consume_token();
                value_bd pos;
                while (true) {
                    if (get_current_token().type == TokenType::END) {
                        throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                    } else if (get_current_token().type == TokenType::R_SQUARE) {
                        break;
                    }
                    id_n.push_back(get_current_token());
                    consume_token();
                }
                if (get_current_token().type != TokenType::R_SQUARE) {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
                consume_token();
                id_n.push_back(getToken(get_current_token().row, get_current_token().col, "END", TokenType::END));
                ASTree* expression_tree = new ASTree(id_n, var_map);
                id_s.push_back(expression_tree->print_no_endl());
                id_s.push_back("]");
                pos = expression_tree->evaluate();
                if (pos.type_tag != "double") {
                    throw EvaluationError("index is not a number.");
                }
                if (pos.Double < 0 || pos.Double >= id_s.size()) {
                    throw EvaluationError("index out of bounds.");
                }
                delete expression_tree;
                return new ArrayNode(temp_row, temp_col, node, pos, id_s, name);
            }
            return node;
        } else if (get_current_token().type == TokenType::BOOLEAN) {
            ASTNode* node = new BooleanNode(get_current_token().row, get_current_token().col, get_current_token().text);
            consume_token();
            return node;
        } else if (get_current_token().type == TokenType::L_SQUARE) {
            std::vector<value_bd> array = {};
            std::vector<std::string> array_ele = {};
            std::string name;
            int square_paren = 1;
            name += get_current_token().text;
            consume_token();
            if (get_current_token().type == TokenType::R_SQUARE) {
                name+=get_current_token().text;
                ASTNode* node = new ArrayNode(get_current_token().row, get_current_token().col, array, array_ele, name);
                consume_token();
                return node;
            }
            std::vector<token> elements = {};
            bool nested_array = false;
            while (square_paren >= 1) {
                if (get_current_token().type == TokenType::END) {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                } else if (get_current_token().type == TokenType::L_SQUARE) {
                    square_paren++;
                    elements.push_back(get_current_token());
                    name += get_current_token().text;
                    consume_token();
                    nested_array = true;
                } else if (get_current_token().type == TokenType::R_SQUARE) {
                    square_paren--;
                    if (square_paren != 0) {
                        elements.push_back(get_current_token());
                    }
                    name += get_current_token().text;
                    consume_token();
                    nested_array = false;
                } else if (get_current_token().type != TokenType::COMMA) {
                    elements.push_back(get_current_token());
                    name += get_current_token().text;
                    consume_token();
                } 
                if ((get_current_token().type == TokenType::COMMA && !nested_array) || square_paren == 0) {
                    elements.push_back(getToken(get_current_token().row, get_current_token().col, "END", TokenType::END));
                    ASTree* tree_eval = new ASTree(elements, var_map);
                    value_bd eval = tree_eval->evaluate();
                    array.push_back(eval);
                    array_ele.push_back(tree_eval->print_no_endl());
                    elements = {};
                    delete tree_eval;
                    if (square_paren != 0) {
                        name += get_current_token().text;
                        consume_token();
                    }
                } else if (get_current_token().type == TokenType::COMMA && nested_array) {
                    elements.push_back(get_current_token());
                    name += get_current_token().text;
                    consume_token();
                }
            }
            ASTNode* node = new ArrayNode(get_current_token().row, get_current_token().col, array, array_ele, name);
            return node;
        } else {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            return nullptr; //will be needing for the case of a unmatched right paren
        }
    } catch (const ParseError& e){
        throw e;
    }
}

