#include "lib/parse.h"

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
    std::string value = tokenized[position].text;
    while (value=="(" || value==")") {
        ++position;
        value = tokenized[position].text;
    }
    position_n = position+1;
    while (tokenized[position_n].type==TokenType::LEFT_PAREN || tokenized[position_n].type==TokenType::RIGHT_PAREN) {
        ++position_n;
    }
    if (tokenized[position].type == TokenType::OPERATOR) {
        Node* new_main = new Node(value, position, tokenized[position].type);
        new_main->l_child = create(tokenized, position_n);
        new_main->r_child = create(tokenized, position_n);
        return new_main;
    } 
    else if (tokenized[position].type == TokenType::NUMBER) {
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
    if (main->type == TokenType::NUMBER) {
        if (main->l_child != nullptr & main->r_child != nullptr) {
            std::cout << "Syntax error on line " << tokenized[main->position].row << " and column " << tokenized[main->position].col << "." << std::endl;
            return 1;
        }
    }
    if (main->type == TokenType::OPERATOR) {
        if (main->l_child == nullptr || main->r_child == nullptr) {
            std::cout << "Syntax error on line " << tokenized[main->position].row << " and column " << tokenized[main->position].col << "." << std::endl;
            return 1;
        }
        if (tokenized[main->position-1].type == TokenType::LEFT_PAREN || 
            tokenized[(main->r_child->position)+1].type == TokenType::RIGHT_PAREN) {
                std::cout << "Syntax error on line " << tokenized[main->position].row << " and column " << tokenized[main->position].col << "." << std::endl;
                return 1;
        }
    }
    if (main->type == TokenType::LEFT_PAREN || main->type == TokenType::RIGHT_PAREN) {
        if (counter(tokenized, TokenType::LEFT_PAREN) != counter(tokenized, TokenType::RIGHT_PAREN)) {
            std::cout << "Syntax error on line " << tokenized[-1].row << " and column " << tokenized[-1].col <<std::endl;
            return 1;
        }
    }
    if (main->l_child==nullptr && main->r_child==nullptr) {
        ;
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
}

void AST::print(Node* main) {
    if (main->l_child==nullptr && main->r_child==nullptr) {
        std::cout << main->text;
    }
    else {
        if (main->l_child!=nullptr) {
            std::cout << "(";
            print(main->l_child);
            std::cout << " ";
        }
        if (main!=nullptr) {
            std::cout << main->text;
        }
        if (main->r_child!=nullptr) {
            std::cout << " ";
            print(main->r_child);
            std::cout << ")";
        }
    }
}