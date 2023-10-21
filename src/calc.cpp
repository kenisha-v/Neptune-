#include <vector>
#include <string>
#include <stdexcept>
#include "lib/lex.h"

// Abstract base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class NumberNode : public ASTNode {
public:
    std::string value;
    explicit NumberNode(const std::string& value) : value(value) {}
};

class IdentifierNode : public ASTNode {
public:
    std::string name;
    explicit IdentifierNode(const std::string& name) : name(name) {}
};

class AssignmentNode : public ASTNode {
public:
    IdentifierNode* id;
    ASTNode* value;
    AssignmentNode(IdentifierNode* id, ASTNode* value) : id(id), value(value) {}
};

class AdditionNode : public ASTNode {
public:
    ASTNode *left, *right;
    AdditionNode(ASTNode* left, ASTNode* right) : left(left), right(right) {}
};

class SubtractionNode : public ASTNode {
public:
    ASTNode *left, *right;
    SubtractionNode(ASTNode* left, ASTNode* right) : left(left), right(right) {}
};

class MultiplicationNode : public ASTNode {
public:
    ASTNode *left, *right;
    MultiplicationNode(ASTNode* left, ASTNode* right) : left(left), right(right) {}
};

class DivisionNode : public ASTNode {
public:
    ASTNode *left, *right;
    DivisionNode(ASTNode* left, ASTNode* right) : left(left), right(right) {}
};

class Parser {
    std::vector<Token> tokens;
    size_t current_token_index = 0;

    Token get_current_token() {
        return tokens[current_token_index];
    }

    void consume_token() {
        current_token_index++;
    }

    void error(const std::string& message) {
        throw std::runtime_error(message);
    }

public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    ASTNode* parse_expression();
    ASTNode* parse_assignment();
    ASTNode* parse_addition_subtraction();
    ASTNode* parse_multiplication_division();
    ASTNode* parse_factor();
};

ASTNode* Parser::parse_expression() {
    return parse_assignment();
}

ASTNode* Parser::parse_assignment() {
    ASTNode* node = parse_addition_subtraction();

    if (get_current_token().tokentype == OPERATOR && get_current_token().text == "=") {
        consume_token();

        if (dynamic_cast<IdentifierNode*>(node) == nullptr) {
            error("Left side of assignment must be an identifier");
        }

        ASTNode* value = parse_assignment();
        return new AssignmentNode(static_cast<IdentifierNode*>(node), value);
    }

    return node;
}

ASTNode* Parser::parse_addition_subtraction() {
    ASTNode* node = parse_multiplication_division();

    while (get_current_token().tokentype == OPERATOR && (get_current_token().text == "+" || get_current_token().text == "-")) {
        if (get_current_token().text == "+") {
            consume_token();
            node = new AdditionNode(node, parse_multiplication_division());
        } else if (get_current_token().text == "-") {
            consume_token();
            node = new SubtractionNode(node, parse_multiplication_division());
        }
    }

    return node;
}

ASTNode* Parser::parse_multiplication_division() {
    ASTNode* node = parse_factor();

    while (get_current_token().tokentype == OPERATOR && (get_current_token().text == "*" || get_current_token().text == "/")) {
        if (get_current_token().text == "*") {
            consume_token();
            node = new MultiplicationNode(node, parse_factor());
        } else if (get_current_token().text == "/") {
            consume_token();
            node = new DivisionNode(node, parse_factor());
        }
    }

    return node;
}

ASTNode* Parser::parse_factor() {
    if (get_current_token().tokentype == L_PAREN) {
        consume_token();
        ASTNode* node = parse_expression();
        if (get_current_token().tokentype != R_PAREN) {
            error("Expected closing parenthesis");
        }
        consume_token();
        return node;
    } else if (get_current_token().tokentype == NUMBER) {
        ASTNode* node = new NumberNode(get_current_token().text);
        consume_token();
        return node;
    } else if (get_current_token().tokentype == IDENTIFIER) {
        ASTNode* node = new IdentifierNode(get_current_token().text);
        consume_token();
        return node;
    } else {
        error("Unexpected token: " + get_current_token().text);
        return nullptr;  // This line is never reached but added to suppress compiler warning
    }
}

int main() {
    // Test the parser here using a sample set of tokens
}
