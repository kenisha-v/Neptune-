#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "lib/lex.h" //to use tokenize
#include "lib/errors.h"

//base class for AST nodes

class ASTNode {
protected:
    int line;
    int column;
public:
    ASTNode(int l, int c) : line(l), column(c) {}

    virtual ~ASTNode() = default;

    virtual double evaluate(){
        return 0.0;
    }

    virtual std::string print(){
        return "";
    }
};

class NumberNode : public ASTNode {
public:
    std::string value;
    explicit NumberNode(int line, int column, const std::string& value)
        : ASTNode(line, column), value(value) {}

    double evaluate(){
        return std::stod(value);
    }

    std::string print() {
        return value;
    }
};


//add the feature of variable already added in map.
class IdentifierNode : public ASTNode {
public:
    std::string name;
    explicit IdentifierNode(int line, int column, const std::string& name) : ASTNode(line, column), name(name){}
    std::string print() {
        return name;
    }
};

class AssignmentNode : public ASTNode {
public:
    IdentifierNode* id;
    ASTNode* value;
    AssignmentNode(int line, int column, IdentifierNode* id, ASTNode* value) : ASTNode(line, column), id(id), value(value){}
    ~AssignmentNode(){
        delete id;
        delete value;
    }
    double evaluate(){
        return value->evaluate();
    }
    std::string print(){
        return "(" + id->print() + " = " + value->print() + ")";
    }
};

class AdditionNode : public ASTNode {
public:
    ASTNode *left, *right;
    AdditionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~AdditionNode(){
        delete left;
        delete right;
    }
    double evaluate(){
        double ans = (left->evaluate() + right->evaluate());
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " + " + right->print() + ")";
    }
};

class SubtractionNode : public ASTNode {
public:
    ASTNode *left, *right;
    SubtractionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~SubtractionNode(){
        delete left;
        delete right;
    }
    double evaluate(){
        double ans = (left->evaluate() - right->evaluate());
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " - " + right->print() + ")";
    }
};

class MultiplicationNode : public ASTNode {
public:
    ASTNode *left, *right;
    MultiplicationNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~MultiplicationNode(){
        delete left;
        delete right;
    }
    double evaluate(){
        double ans = (left->evaluate() * right->evaluate());
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " * " + right->print() + ")";
    }
};

class DivisionNode : public ASTNode {
public:
    ASTNode *left, *right;
    DivisionNode(int line, int column, ASTNode* left, ASTNode* right) : ASTNode(line, column), left(left), right(right){}
    ~DivisionNode(){
        delete left;
        delete right;
    }
    double evaluate() {
        double rightValue = right->evaluate();
        if (rightValue == 0.0) {
            throw EvaluationError("Division by zero");
        }
        double ans = left->evaluate() / rightValue;
        return ans;
    }
    std::string print(){
        return "(" + left->print() + " / " + right->print() + ")";
    }
};

class ASTree {
    std::vector<token> tokens;
    size_t current_token_index = 0;
    ASTNode* head;

    token get_current_token()   {return tokens[current_token_index];}
    void consume_token()        {current_token_index++;}
    
    ASTNode* parse_expression();
    ASTNode* parse_assignment();
    ASTNode* parse_addition_subtraction();
    ASTNode* parse_multiplication_division();
    ASTNode* parse_factor();

public:
    
    ASTree(const std::vector<token>& Tokens)    { tokens=Tokens; head = parse_expression(); } //for variable identification, add unordered map pointer.
    double ASTree::evaluate()                   { return head->evaluate(); }
    void print()                                { std::cout << head->print() << std::endl;}
    ~ASTree()                                   { delete head; }
};

ASTNode* ASTree::parse_expression() {
    return parse_assignment();
}

ASTNode* ASTree::parse_assignment() {
    ASTNode* node = parse_addition_subtraction();

    if (get_current_token().tokentype == OPERATOR && get_current_token().text == "=") {
        int temp_row            = get_current_token().row;
        int temp_col            = get_current_token().col;
        std::string temp_text   = get_current_token().text;
        consume_token();

        if (dynamic_cast<IdentifierNode*>(node) == nullptr) {
            throw ParseError(temp_row, temp_col, temp_text);//
        }

        ASTNode* value = parse_assignment();
        return new AssignmentNode(temp_row, temp_col, static_cast<IdentifierNode*>(node), value);
    }

    return node;
}

ASTNode* ASTree::parse_addition_subtraction() {
    ASTNode* node = parse_multiplication_division();

    while (get_current_token().tokentype == OPERATOR && (get_current_token().text == "+" || get_current_token().text == "-")) {
        if (get_current_token().text == "+") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            std::string temp_text   = get_current_token().text;
            consume_token();
            node = new AdditionNode(temp_row, temp_col, node, parse_multiplication_division());
        } else if (get_current_token().text == "-") {
            int temp_row            = get_current_token().row;
            int temp_col            = get_current_token().col;
            std::string temp_text   = get_current_token().text;
            consume_token();
            node = new SubtractionNode(temp_row, temp_col, node, parse_multiplication_division());
        }
    }

    return node;
}

ASTNode* ASTree::parse_multiplication_division() {
    ASTNode* node = parse_factor();

    while (get_current_token().tokentype == OPERATOR && (get_current_token().text == "*" || get_current_token().text == "/")) {
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
        }
    }

    return node;
}

ASTNode* ASTree::parse_factor() {
    if (get_current_token().tokentype == L_PAREN) {
        consume_token();
        ASTNode* node = parse_expression();
        if (get_current_token().tokentype != R_PAREN) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token().text);
        }
        consume_token();
        return node;
    } else if (get_current_token().tokentype == NUMBER) {
        ASTNode* node = new NumberNode(get_current_token().row, get_current_token().col, get_current_token().text);
        consume_token();
        return node;
    } else if (get_current_token().tokentype == VARIABLES) {
        ASTNode* node = new IdentifierNode(get_current_token().row, get_current_token().col, get_current_token().text);
        consume_token();
        return node;
    } else {
        throw ParseError(get_current_token().row, get_current_token().col, get_current_token().text);
        return nullptr; //will probably be needing to add the case of a unmatched right paren
    }
}


int main() {
    std::string input;
    std::unordered_map<std::string, double> Variable_Values; 
    ASTree* curr_tree = nullptr;

    while (true){
        try {
            std::getline(std::cin, input);
            if (std::cin.eof()) {
                break;
            }
            std::vector<token> input_tokens = tokenize(input);
            delete curr_tree;
            curr_tree = new ASTree(input_tokens);
        } catch (const ParseError& e) {
            std::cout << e.what() << std::endl;
            delete curr_tree;
            curr_tree = nullptr;
            continue;
        }
        curr_tree->print();
        std::cout << curr_tree->evaluate() << std::endl;
    }

    delete curr_tree;
}
