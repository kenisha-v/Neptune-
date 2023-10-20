#include "lib/parse.h"

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
    int i = 0;
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
                //operator cannot be followed by right parentheses
                if(tokenized[i+1].type == TokenType::RIGHT_PAREN){
                    throw ParseError(tokenized[i+1].row, tokenized[i+1].col, tokenized[i+1]);
                }
                curr_ptr->text = curr_token.text;
            } 
            //if token is a number add it as a child to current node. if it is the only node point head to it
            else if (curr_token.type == TokenType::NUMBER) {
                if(curr_ptr){
                    curr_ptr->children.push_back(new Node(curr_ptr, curr_token.text));
                } 
                else if (head == nullptr) {
                    head = new Node(nullptr, curr_token.text);
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
        } else {
            throw EvaluationError("Invalid operator: " + node->text);
        }
    } else {
        return std::stod(node->text);
    }
}

int main(){
    std::string line;
    int row = 1;
    int col = 1;
    while(std::getline(std::cin, line)) {
        try {
            AST ast(tokenize(line, row, col));
            ast.printAST(ast.head);
            std::cout << "\n" << ast.evaluate(ast.head) << std::endl;
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
        ++row;
    }

    return 0;
}

