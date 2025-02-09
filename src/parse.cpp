#include "lib/parse.h"
#include "lib/errors.h"

char ar_op[9] = {'=','+', '-', '*', '/', '(', ')', ' ', '\t'};

Node::Node(Node* parent, std::string text) {
    this->text = text;
    this->parent = parent;
    this->children = {};
}

Node::~Node(){
    for(auto child: children){
        delete child;
    }
}

//constructor
AST::AST(std::vector<token> tokenized) {
    Node* curr_ptr = nullptr;
    this->head = nullptr;
    size_t i = 0;
    token curr_token = tokenized[i];
    bool started = false;
    //try block to deal with memory leaks when constructor encounters a parse error
    try {
        //check if tokenize only has end token
        if(tokenized.size()<= 1){
            throw ParseError(curr_token.row, curr_token.col, curr_token);
        }
        //loop to traverse through each token and create tree
        while (curr_token.type != TokenType::END) {
            //if statement to check that there are no more tokens after all "(" have been closed by ")"
            if(curr_ptr == nullptr && curr_token.type != TokenType::END && started){
                throw ParseError(curr_token.row, curr_token.col, curr_token);
            }
            //above condition would always be true without this bool. declares that ast construction has started
            started = true;
            //makes sure that left parentheses is only followed by an operator
            if(curr_ptr != nullptr && curr_ptr->text == "" && curr_token.type != TokenType::OPERATOR){
                throw ParseError(curr_token.row, curr_token.col, curr_token);
            }
            //if token is left parentheses, create an empty node and manage children and parent connections (does not insert text in node yet as that requires the operator which we get in the next iteration)
            if (curr_token.type == TokenType::LEFT_PAREN) {
                //if the operator is an "=" and already has a digit or another operator as a child throw error
                if(curr_ptr && curr_ptr->text == "="){
                    for(auto child: curr_ptr->children){
                        if(isdigit(child->text[0]) || child->text == "+" || child->text == "-" || child->text == "*" || child->text == "/"){
                            throw ParseError(tokenized[i+1].row, tokenized[i+1].col, tokenized[i+1]);
                        }
                    }
                }
                Node* tmp_ptr = new Node(curr_ptr, "");
                if(curr_ptr != nullptr){
                    curr_ptr->children.push_back(tmp_ptr);
                }
                else{
                    head = tmp_ptr;
                }
                curr_ptr = tmp_ptr;
            }
            //if token is right parentheses, shifts current pointer to parent which helps check that number of "(" equals ")"
            else if (curr_token.type == TokenType::RIGHT_PAREN) {
                //extra ")" after all parentheses have been closed
                if (curr_ptr == nullptr) {
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                //if "=" operator has less than two child throw error
                if(curr_ptr->text == "=" && curr_ptr->children.size() <= 1){
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                curr_ptr = curr_ptr->parent;
            }
            //if token is operator adds it as a text to the empty node we created during left parentheses
            else if (curr_token.type == TokenType::OPERATOR) {
                //no parentheses before operator (start case as i-1 becomes negative)
                if(i == 0){
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                //no parentheses before operator in general
                else if(tokenized[i-1].type != TokenType::LEFT_PAREN){
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                //if operator is "=" and next token is not a variable throw error
                if (curr_token.text == "=") {
                    if(i+1 >= tokenized.size() || tokenized[i+1].type != TokenType::VARIABLES){
                        throw ParseError(tokenized[i+1].row, tokenized[i+1].col, tokenized[i+1]);
                    }
                }
                //operator cannot be followed by right parentheses
                if(tokenized[i+1].type == TokenType::RIGHT_PAREN){
                    throw ParseError(tokenized[i+1].row, tokenized[i+1].col, tokenized[i+1]);
                }
                curr_ptr->text = curr_token.text;
            }
            //if token is a number add it as a child to current node. if it is the only node point head to it
            else if (curr_token.type == TokenType::NUMBER) {
                if(curr_ptr){
                    //if the operator is an "=" and already has a digit or another operator as a child no new numbers can be added
                    if(curr_ptr->text == "="){
                        for(auto child: curr_ptr->children){
                            if(isdigit(child->text[0]) || child->text == "+" || child->text == "-" || child->text == "*" || child->text == "/"){
                                throw ParseError(curr_token.row, curr_token.col, curr_token);
                            }
                        }
                    }
                    curr_ptr->children.push_back(new Node(curr_ptr, curr_token.text));
                }
                else if (head == nullptr) {
                    head = new Node(nullptr, curr_token.text);
                }
            }
            //if token is a variable add it as a child to current node. if it is the only node throw an error
            else if (curr_token.type == TokenType::VARIABLES) {
                if(curr_ptr){
                    //if the operator is an "=" and already has a digit or another operator as a child no new variables can be added
                    if(curr_ptr->text == "="){
                        for(auto child: curr_ptr->children){
                            if(isdigit(child->text[0]) || child->text == "+" || child->text == "-" || child->text == "*" || child->text == "/"){
                                throw ParseError(curr_token.row, curr_token.col, curr_token);
                            }
                        }
                    }
                    curr_ptr->children.push_back(new Node(curr_ptr, curr_token.text));
                }
                else if (head == nullptr) {
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
            }
            i++;
            curr_token = tokenized[i];
        }
        // checking parenthesis balanced
        if (curr_ptr != nullptr) {
            throw ParseError(curr_token.row, curr_token.col + curr_token.text.length()-1, curr_token);
        }
    } catch(const ParseError& e){
        delete head;
        throw e;
    }
}


AST::~AST() {
    delete head;
}

void AST::printAST(Node* node) {
    if (node == nullptr) {
        return;
    }
    if (!node->children.empty()) {
        std::cout << "(";
        for (size_t index = 0; index < node->children.size(); index++) {
            printAST(node->children.at(index));
            if(index != node->children.size() - 1){
                //converting string to double if text is a digit to deal with formatting issues
                if(isdigit(node->text.at(0))){
                    std::cout << " " << stod(node->text) << " ";
                }
                else{
                    std::cout << " " << node->text << " ";
                }
            }
        }
        std::cout << ")";
    } else {
        //converting string to double if text is a digit to deal with formatting issues
        if(isdigit(node->text.at(0))){
            std::cout << stod(node->text);
        }
        else{
            std::cout << node->text;
        }
    }

}

double AST::evaluate(Node* node) {
    if (node == nullptr) {
        throw EvaluationError("Null node encountered");
    }

    if (!node->children.empty()) {
        if (node->text == "+") {
            double result = 0.0;
            if (node->children.size() == 0) {
                throw EvaluationError("Invalid number of operands for operator: " + node->text);
            }
            for (Node* child : node->children) {
                result += evaluate(child);
            }
            return result;
        } else if (node->text == "-") {
            if (node->children.size() == 0) {
                throw EvaluationError("Invalid number of operands for operator: " + node->text);
            }
            double result = evaluate(node->children[0]);
            for (size_t i = 1; i < node->children.size(); ++i) {
                result -= evaluate(node->children[i]);
            }
            return result;
        } else if (node->text == "*") {
            double result = 1.0;
            if (node->children.size() == 0) {
                throw EvaluationError("Invalid number of operands for operator: " + node->text);
            }
            for (Node* child : node->children) {
                result *= evaluate(child);
            }
            return result;
        } else if (node->text == "/") {
            if (node->children.size() < 2) {
                throw EvaluationError("Invalid number of operands for operator: " + node->text);
            }
            double result = evaluate(node->children[0]);
            for (size_t i = 1; i < node->children.size(); ++i) {
                double denominator = evaluate(node->children[i]);
                if (denominator == 0) {
                    throw EvaluationError("division by zero.");
                }
                result /= denominator;
            }
            return result;
        } else if (node->text == "=") {
            if (node->children.size() <= 1) {
                throw EvaluationError("Assignment requires at least two operands.");
            }
            double value;
            for(auto child : node->children){
                if(child->text == "+" || child->text == "-" || child->text == "*" || child->text == "/"){
                    value = evaluate(child);
                }
            }
            for(auto child : node->children){
                if(isdigit(child->text[0])){
                    value = std::stod(child->text);
                    break;
                }
            }
            for (auto child : node->children){
                if(!isdigit(child->text[0])){
                    symbolTable[child->text] = value;
                }
            }

            // Return the assigned value
            return value;
        } else {
            throw EvaluationError("Invalid operator: " + node->text);
        }
    } else {
        if(isdigit(node->text[0])){
            return std::stod(node->text);
        }
        else{
            if(symbolTable.find(node->text) != symbolTable.end()){
                return symbolTable[node->text];
            }
            else{
                throw EvaluationError("unknown identifier " + node->text);
            }
        }
    }
}
std::map<std::string, double> AST::getVariables(){
    return symbolTable;
}
void AST::updateVariables(std::map<std::string, double> symbolTable){
    this->symbolTable = symbolTable;
}

int main(){
    std::string input;
    std::map<std::string, double> symbolTable;
    char ch;
    std::vector<std::string> lines;
    std::vector<char> para;
    bool tree = false;

    // While loop to take in inputs for multiple trees
    while (std::cin.get(ch)) {
        if (tree == true && ch == '\n') {
            lines.push_back(input);
            input = "";
            tree = false;
            continue;
        }
        if (ch == '(') {
            if (!input.empty() && para.empty()) {
                lines.push_back(input);
                input = "";
            }
            para.push_back(ch);
        }
        else if (ch == ')') {
            para.pop_back();
            if (para.size() == 0) {
                input += ch;
                tree = true;
                continue;
            }
        }
        input += ch;
    }

    // Helpful if there is a missing )
    if (!input.empty()) {
        lines.push_back(input);
        input = "";
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        try {
            if (tokenize(lines[i]).size()<=1) {
                continue;
            }
            AST ast(tokenize(lines[i]));
            ast.updateVariables(symbolTable);
            ast.printAST(ast.head);
            std::cout << "\n" << ast.evaluate(ast.head) << std::endl;
            symbolTable = ast.getVariables();
        } catch(const SyntaxError& e) {
            std::cout << e.what() << std::endl;
            return 1;
        } catch(const ParseError& e){
            std::cout << e.what() << std::endl;
            return 2;
        } catch(const EvaluationError& e){
            std::cout << e.what() << std::endl;
            return 3;
        }
    }

    return 0;
}
