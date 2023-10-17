#include "lib/parse.h"

void printTokens(const std::vector<token>& tokens) {
    
    for(const token& token : tokens) {
        std::cout << std::setw(4) << std::right << token.row << "   " << std::setw(2) << std::right << token.col << "  " << token.text << std::endl;
    }
}

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

void AST::debugPrintAST(Node* node, int level) {
    if (node == nullptr) {
        std::cout << std::string(level, ' ') << "NULL NODE" << std::endl;
        return;
    }
    std::cout << std::string(level, ' ') << node->text << std::endl;
    for (Node* child : node->children) {
        debugPrintAST(child, level + 2);
    }
}

//constructor
AST::AST(std::vector<token> tokenized) {
    Node* curr_ptr = nullptr;
    this->head = nullptr;
    int i = 0;
    token curr_token = tokenized[i];
    bool started = false;
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
            else if (curr_token.type == TokenType::RIGHT_PAREN) {
                //extra ")" after all parentheses have been closed
                if (curr_ptr == nullptr) {
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                curr_ptr = curr_ptr->parent;
            } 
            else if (curr_token.type == TokenType::OPERATOR) {
                if(i == 0){ //no parentheses before operator while starting
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                else if(tokenized[i-1].type != TokenType::LEFT_PAREN){ //no parentheses before operator in general
                    throw ParseError(curr_token.row, curr_token.col, curr_token);
                }
                if(tokenized[i+1].type == TokenType::RIGHT_PAREN){
                    throw ParseError(tokenized[i+1].row, tokenized[i+1].col, tokenized[i+1]);
                }
                curr_ptr->text = curr_token.text;
            } 
            else if (curr_token.type == TokenType::NUMBER) {
                if(curr_ptr){
                    curr_ptr->children.push_back(new Node(curr_ptr, curr_token.text));
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
        std::cout << "DEBUG: Constructed AST:" << std::endl;
        printTokens(tokenize(input));
        std::cout << "DEBUG: End of AST" << std::endl << std::endl;

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

    return 0;
}

