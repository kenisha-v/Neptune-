#include "lib/parse.h"
#include "lib/lex.h"

Node::Node() {
    text="";
    l_child = r_child = nullptr;
}

Node::Node(std::string text, int position, TokenType type, Node* l_child, Node* r_child) {
    this->text = text;
    this->position = position;
    this->type = type;
    this->l_child = l_child;
    this->r_child = r_child;
}

AST::AST() {
    this->main = nullptr;
}

void AST::construct(std::vector<token> tokenized){
    this->main = create(tokenized);
}

Node* AST::create(std::vector<token> tokenized, int position){
    if ((size_t)position>tokenized.size()){
        return nullptr;
    }
    TokenType type = tokenized[position].type;
    std::string value = tokenized[position].text;
    while (type==TokenType::LEFT_PAREN || type==TokenType::RIGHT_PAREN){
        ++position;
        type = tokenized[position].type;
        value = tokenized[position].text;
    }
    next_position = position+1;
    
    if (type == TokenType::OPERATOR) {
        Node* new_main = new Node(value, position, tokenized[position].type);
        new_main->l_child = create(tokenized, next_position);
        new_main->r_child = create(tokenized, next_position);
        return new_main;
    } 
    else if (type == TokenType::NUMBER) {
        Node* new_main = new Node(value, position, tokenized[position].type);
        return new_main;
    }
    else {
        return nullptr;
    }
}

int AST::counter(std::vector<token> tokenized, TokenType type) {
    int cnt = 0;
    for (size_t i=0; i<tokenized.size(); ++i) {
        if (tokenized[i].type == type) {
            ++cnt;
        }
    }
    return cnt;
}

int AST::syntax_error(std::vector<token> tokenized, Node* main) {
    //redundant as number can never have a l_child or r_child due to construct
    // if (main->type == TokenType::NUMBER) {
    //     if (main->l_child != nullptr & main->r_child != nullptr) {
    //         std::cout << "Syntax error on line " << tokenized[main->position].row << " and column " << tokenized[main->position].col << "." << std::endl;
    //         return 1;
    //     }
    // }
    if (main->type == TokenType::OPERATOR) {
        if (main->l_child == nullptr || main->r_child == nullptr || tokenized[main->position-1].type != TokenType::LEFT_PAREN) { 
            std::cout << "Unexpected token at line " << tokenized[main->position].row << " and column " << tokenized[main->position].col << ": " << tokenized[main->position].text << std::endl;
            return 2;
        }
        // if (tokenized[main->position-1].type == TokenType::LEFT_PAREN || 
        //     tokenized[(main->r_child->position)+1].type == TokenType::RIGHT_PAREN) {
        //         std::cout << "Syntax error on line " << tokenized[main->position].row << " and column " << tokenized[main->position].col << "." << std::endl;
        //         return 1;
        // }
    }
    if (main->type == TokenType::LEFT_PAREN || main->type == TokenType::RIGHT_PAREN) {
        if (counter(tokenized, TokenType::LEFT_PAREN) != counter(tokenized, TokenType::RIGHT_PAREN)) {
            std::cout << "Syntax error on line " << tokenized[-1].row << " and column " << tokenized[-1].col <<std::endl;
            return 2;
        }
    }
    if (main->l_child==nullptr && main->r_child==nullptr) {
        ; //what??????
    }
    else {
        if (main->l_child!=nullptr) {
            syntax_error(tokenized, main->l_child);
        }
        if (main!=nullptr) {
            ;
        }
        if (main->r_child!=nullptr) {
            syntax_error(tokenized, main->r_child);
        }
    }
    return 0;
}

void AST::print(Node* main) {
    if (main->l_child==nullptr && main->r_child==nullptr) {
        infix += main->text;
    }
    else {
        if (main->l_child!=nullptr) {
            infix += "(";
            print(main->l_child);
            infix += " ";
        }
        if (main!=nullptr) {
            infix += main->text;
        }
        if (main->r_child!=nullptr) {
            infix += " ";
            print(main->r_child);
            infix += ")";
        }
    }
}

double AST::evaluate(Node* main){
    if (main->type == TokenType::NUMBER) {
        return std::stod(main->text);
    }

    // If the node is an operator, perform the operation
    if (main->type == TokenType::OPERATOR) {
        double left_result = evaluate(main->l_child);
        double right_result = evaluate(main->r_child);

        if (main->text == "+") {
            return left_result + right_result;
        } else if (main->text == "-") {
            return left_result - right_result;
        } else if (main->text == "*") {
            return left_result * right_result;
        } else if (main->text == "/") {
            if (right_result == 0) {
                // Handle division by zero
                std::cout << "Runtime error: Division by zero." << std::endl;
                exit(3);
            }
            return left_result / right_result;
        }
    }
    return 0;
}

int main(){
//     std::string input;
//     char ch;
//     vector<token> tokens;
//     while (std::cin.get(ch)) { //reading input character by character to ensure pre-eof chararcter detection.
//         input += ch;
//     }

//     try {
//         tokens = tokenize(input);
//     } catch(const SyntaxError& e) {
//         std::cout << e.what() << std::endl;
//         return 1;
//     }

//     AST ast;
//     ast.construct(tokens);
//     int error = ast.syntax_error(tokens, ast.main);
//     if(error == 2){
//         return 2;
//     }
//     ast.print(ast.main);
//     std::cout << ast.infix << std::endl;
//     std::cout << ast.evaluate(ast.main) << std::endl;

     return 0;
}