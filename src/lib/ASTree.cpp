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
        if (dynamic_cast<IdentifierNode*>(id) == nullptr && dynamic_cast<ArrayNode*>(id) == nullptr) {
            throw EvaluationError("invalid assignee.");
        }
        if (dynamic_cast<IdentifierNode*>(id) == nullptr) {
            ArrayNode* id_n = static_cast<ArrayNode*>(id);
            value_bd solved_value_right_node = value->evaluate(var_map);
            std::vector<value_bd> array_left = (*var_map)[id_n->name].array;
            // id_n->evaluate(var_map) = solved_value_right_node;
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
        if(left->evaluate(var_map).type_tag != right->evaluate(var_map).type_tag) {
            return value_bd("bool", false);
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
}

ArrayNode::ArrayNode(int line, int column, ASTNode* node, value_bd position, std::vector<std::string> array_ele, std::string name) : ASTNode(line, column), node(node), array_ele(array_ele), position(position), name(name) {}
    
ArrayNode::ArrayNode(int line, int column, std::vector<value_bd> array): ASTNode(line, column){
    this->array = {};
    for (size_t i = 0; i< array.size(); ++i ) {
        this->array.push_back(array[i]);
    }
    this->node = nullptr;
}

ArrayNode::~ArrayNode(){
        delete node;
    }
    
value_bd ArrayNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
        // if (node != nullptr) {
        //     std::string array_str;
        //     array_str += "[";
        //     for (size_t i = 0; i< array.size()-1; ++i ) {
        //         if (array[i].type_tag == "bool") {
        //             if (array[array.size()-1].Bool) {
        //                 array_str+="true";
        //             } else {
        //                 array_str+="false";
        //             }
        //         } else if (array[i].type_tag == "double"){
        //             std::ostringstream os;
        //             os << array[i].Double;
        //             array_str+=os.str();
        //         } else if (array[i].type_tag == "array"){
        //             ArrayNode* array_ele_node = new ArrayNode(0, 0, array[i].array);
        //             array_str += array_ele_node->print();
        //         } else {
        //             ;
        //         }
        //         array_str+=", ";
        //     }
        //     if (array[array.size()-1].type_tag == "bool") {
        //         if (array[array.size()-1].Bool) {
        //             array_str+="true";
        //         } else {
        //             array_str+="false";
        //         }
        //     } else if (array[array.size()-1].type_tag == "double"){
        //         std::ostringstream os;
        //         os << array[array.size()-1].Double;
        //         array_str+=os.str();
        //     } else {
        //         ArrayNode* array_ele_node = new ArrayNode(0, 0, array[array.size()-1].array);
        //         array_str += array_ele_node->print();
        //     }
        //     array_str += "]";
        //     return value_bd("array",array_str);
        // }
        if (node != nullptr) {
            value_bd val = (*var_map)[node->print()];
            if (node->print()[0] == '[') {
                val = node->evaluate(var_map);
            }
            if (position.type_tag!="double") {
                throw EvaluationError("expression not valid");
            }
            // for (size_t i = 0; i< position.size();++i) {
            //     if (!isdigit(position[i])) {
            //         throw EvaluationError("index is not a number");
            //     }
            // }
            // int pos = std::stod(position);
            if (position.Double < 0 || position.Double >= val.array.size()) {
                throw EvaluationError("index out of bounds.");
            }
            return val.array[position.Double];
        }
        // std::cout << array.size() << "\n";
        return value_bd("array", array, array_ele);
    }
    
std::string ArrayNode::print(){
        std::string array_str;
        if (node != nullptr){
            // value_bd val = evaluate(variable_map);
            // std::cout << "seg \n";
            // if(val.array[position].type_tag == "bool") {
            //     if(val.array[position].Bool) {
            //         array_str += "true";
            //     } else {
            //         array_str += "false";
            //     }
            // } else if(val.array[position].type_tag == "double") {
            //     array_str += val.array[position].Double;
            // } else {
            //     array_str += evaluate_print(val.array[position].array);
            // }
            for (size_t i = 0; i< array_ele.size(); ++i ) {
                array_str+= array_ele[i];
            }
            return array_str;
        }
        array_str+="[";
        for (size_t i = 0; i< array_ele.size()-1; ++i ) {
            array_str+= array_ele[i];
            array_str+=", ";
        }
        array_str+= array_ele[array_ele.size()-1];
        array_str+="]";
        // array_str += "[";
        // for (size_t i = 0; i< array.size()-1; ++i ) {
        //     if (array[i].type_tag == "bool") {
        //         if (array[array.size()-1].Bool) {
        //             array_str+="true";
        //         } else {
        //             array_str+="false";
        //         }
        //     } else if (array[i].type_tag == "double"){
        //         std::ostringstream os;
        //         os << array[i].Double;
        //         array_str+=os.str();
        //     } else if (array[i].type_tag == "array"){
        //         ArrayNode* array_ele_node = new ArrayNode(0, 0, array[i].array);
        //         array_str += array_ele_node->print();
        //     } else {
        //         ;
        //     }
        //     array_str+=", ";
        // }
        // if (array[array.size()-1].type_tag == "bool") {
        //     if (array[array.size()-1].Bool) {
        //         array_str+="true";
        //     } else {
        //         array_str+="false";
        //     }
        // } else if (array[array.size()-1].type_tag == "double"){
        //     std::ostringstream os;
        //     os << array[array.size()-1].Double;
        //     array_str+=os.str();
        // } else {
        //     ArrayNode* array_ele_node = new ArrayNode(0, 0, array[array.size()-1].array);
        //     array_str += array_ele_node->print();
        // }
        // array_str += "]";
        return array_str;
    }

std::string ArrayNode::evaluate_print(std::vector<value_bd> arr) {
    std::string str;
    str+="[";
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
    } else {
        ArrayNode* arr_ele = new ArrayNode(0,0, arr[arr.size()-1].array);
        str+=arr_ele->evaluate_print(arr[arr.size()-1].array);
        delete arr_ele;
        arr_ele = nullptr;
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
        // std::cout << "staring parse_Lor - LHS of =\n";
        node = parse_Lor();

        if (get_current_token().type == TokenType::OPERATOR && get_current_token().text == "=") {
            // std::cout << "enetered if in parse_assignment\n";
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            
            value = parse_assignment();
            // std::cout << "created?\n";
            return new AssignmentNode(temp_row, temp_col, node, value);
        } else if (get_current_token().type == TokenType::L_SQUARE){
            // std::cout << "enetered else if in parse_assignment\n";
            // int temp_row            = get_current_token().row;
            // int temp_col            = get_current_token().col;
            // token temp_token        = get_current_token();
            // consume_token();
            // int pos = std::stod(get_current_token().text);
            // consume_token();
            // if (get_current_token().type != TokenType::R_SQUARE) {
            //     throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            // }
            // consume_token();
            // return new ArrayNode(temp_row, temp_col, node, pos, );
            ArrayNode* nod = static_cast<ArrayNode*>(node);
            std::string name = nod->name;
            // ASTNode* node = new IdentifierNode(get_current_token().row, get_current_token().col, get_current_token().text);
            std::vector<std::string> id_s;
            id_s.push_back(nod->print());
            std::vector<token> id_n;
            if (get_current_token().type == TokenType::L_SQUARE){
                // std::cout << "new one - enetered else if in parse_assignment\n";
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
                // delete expression_tree;
                return new ArrayNode(temp_row, temp_col, nod, pos, id_s, name);
            }
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
        // std::cout << "4\n";
        
        while (get_current_token().type == TokenType::L_OPERATOR && get_current_token().text == "|") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            node = new LorNode(temp_row, temp_col, node, parse_Lxor());
        }
        // std::cout << "4 done\n";
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
        // std::cout << "3\n";
        
        while (get_current_token().type == TokenType::L_OPERATOR && get_current_token().text == "^") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            node = new LxorNode(temp_row, temp_col, node, parse_Land());
        }
        
        // std::cout << "3 done\n";
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
        // std::cout << "2\n";
        
        while (get_current_token().type == TokenType::L_OPERATOR && get_current_token().text == "&") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            token temp_token        = get_current_token();
            consume_token();
            node = new LandNode(temp_row, temp_col, node, parse_equality());
        }
        
        // std::cout << "2 done\n";
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
        // std::cout << "1\n";

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

        // std::cout << "1 done\n";
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
        // std::cout << "token in > :"<< get_current_token().text<<"\n";

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

        // std::cout << "returning >\n";
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
                // std::cout << "new one - enetered else if in parse_assignment\n";
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
            std::vector<token> elements;
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
                    if (square_paren != 0) {
                        name += get_current_token().text;
                        consume_token();
                    }
                    delete tree_eval;
                } else if (get_current_token().type == TokenType::COMMA && nested_array) {
                    elements.push_back(get_current_token());
                    name += get_current_token().text;
                    consume_token();
                }
            }
            ASTNode* node = new ArrayNode(get_current_token().row, get_current_token().col, array, array_ele, name);
            return node;
            // while (square_paren >= 1) {
            //     if (get_current_token().type == TokenType::R_SQUARE) {
            //         square_paren--;
            //         consume_token();
            //         break;
            //     }
            //     // std::cout <<"looping "<< get_current_token().text<<" \n";
            //     if (get_current_token().type == TokenType::END) {
            //         throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            //     }
            //     if (get_current_token().type != TokenType::COMMA) {
            //         // std::cout <<"appending\n";
            //         array->push_back(get_current_token());
            //         // std::cout <<"appended\n";
            //         consume_token();
            //         if (get_current_token().type == TokenType::R_SQUARE) {
            //             square_paren--;
            //             consume_token();
            //             // std::cout <<"trying to loop "<< get_current_token().text<<" \n";
            //         } else if (get_current_token().type == TokenType::L_SQUARE) {
            //             square_paren++;
            //             consume_token();
            //             // std::cout <<"trying to loop "<< get_current_token().text<<" \n";
            //         }
            //     } else {
            //         consume_token();
            //     }
            // }
        } else {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            return nullptr; //will be needing for the case of a unmatched right paren
        }
    } catch (const ParseError& e){
        throw e;
    }
}

