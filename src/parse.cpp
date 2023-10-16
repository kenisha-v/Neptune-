#include "lib/parse.h"
#include "lib/lex.h"

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


AST::AST(std::vector<token> tokenized) {
    Node* curr_ptr = new Node(nullptr, "root");
    this->head = curr_ptr;
    int i = 0;
    token curr_token = tokenized[i];
    try {
        if(tokenized.size()<= 1){
            throw ParseError(curr_token.row, curr_token.col, curr_token);
        }
        while (curr_token.type != TokenType::END) {
            if(curr_ptr->text == "" && curr_token.type != TokenType::OPERATOR){
                throw ParseError(curr_token.row, curr_token.col, curr_token);
            }
            if (curr_token.type == TokenType::LEFT_PAREN) {
                if(tokenized[i+1].type != TokenType::OPERATOR){
                    throw ParseError(tokenized[i+1].row, tokenized[i+1].col, tokenized[i+1]);
                }
                Node* tmp_ptr = new Node(curr_ptr, "");
                curr_ptr->children.push_back(tmp_ptr);
                curr_ptr = tmp_ptr;
            } else if (curr_token.type == TokenType::RIGHT_PAREN) {
                if (curr_ptr->parent == nullptr) {
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                if(curr_ptr->children.size() < 2){
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                curr_ptr = curr_ptr->parent;
            } else if (curr_token.type == TokenType::OPERATOR) {
                if(tokenized[i+1].type == TokenType::OPERATOR){
                    throw ParseError(tokenized[i+1].row, tokenized[i+1].col, tokenized[i+1]);
                }
                curr_ptr->text = curr_token.text;
            } else if (curr_token.type == TokenType::NUMBER) {
                curr_ptr->children.push_back(new Node(curr_ptr, curr_token.text));
            }
            i++;
            curr_token = tokenized[i];
        }

        if (curr_ptr != head || curr_ptr->parent != nullptr) {
            throw ParseError(curr_token.row, curr_token.col, curr_token);
        }
    } catch(const ParseError& e){
        delete head;
        throw e;
    }
}


AST::~AST() {
    delete head;
    //delete curr_ptr;
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
        } else {
            throw EvaluationError("Invalid operator: " + node->text);
        }
    } else {
        return std::stod(node->text);
    }
}

int main(){
    std::string input;
    char ch;

    while (std::cin.get(ch)) { //reading input character by character to ensure pre-eof chararcter detection.
        input += ch;
    }

    try {
        AST ast(tokenize(input));
        if(!ast.head->children.empty()){
            ast.printAST(ast.head->children[0]);
            std::cout << "\n" << ast.evaluate(ast.head->children[0]) << std::endl;
        }
        else{
            // std::cout << "Unexpected token at line 1 column 1: END" << std::endl;
            ast.printAST(ast.head);
            return 2;
        }
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

    return 0;
}