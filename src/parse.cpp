#include "lib/parse.h"

Node::Node() {
    text="";
    l_child = r_child = nullptr;
}

Node::Node(std::string text, Node* l_child, Node* r_child) {
    this->text = text;
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
    expression.append(value+" ");
    while (value=="(" || value==")") {
        ++position;
        value = tokenized[position].text;
    }
    position_n = position+1;
    while (tokenized[position_n].text=="(" || tokenized[position_n].text==")") {
        ++position_n;
    }
    if (tokenized[position].type == TokenType::OPERATOR) {
        Node* new_main = new Node(value);
        new_main->l_child = create(tokenized, position_n);
        new_main->r_child = create(tokenized, position_n);
        return new_main;
    } 
    else if (tokenized[position].type == TokenType::NUMBER) {
        Node* new_main = new Node(value);
        return new_main;
    }
    else {
        return nullptr;
    }
}

int AST::error(std::string expression) {
    
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