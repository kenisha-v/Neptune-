#include "STree.hpp"

//-----------------

SNode::SNode(ASTree* exp, SNode* next): expression(exp), next(next) {}
SNode::~SNode() {
    delete expression;
    delete next;
}
void SNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    (void)var_map;
    return;
}

//-----------------

ExpressionNode::ExpressionNode(ASTree* exp, SNode* next): SNode(exp, next) {
    //std::cout<< "Creating ExpressionNode.... ";
    //exp->print();
}
void ExpressionNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    expression->evaluate();
    if (next!= nullptr){
        next->evaluate(var_map);
    }
}
ExpressionNode::~ExpressionNode() {
    
}

//-----------------

WhileNode::WhileNode(ASTree* exp, SNode* next, STree* t): SNode(exp, next), trueBranch(t) {
    //std::cout<< "Creating WhileNode.... ";
    //exp->print();
}
void WhileNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    value_bd exp_eval = expression->evaluate();
    if (exp_eval.type_tag != "bool") {
        throw EvaluationError("condition is not a bool.");
    }
    while (true){
        exp_eval = expression->evaluate();
        if (exp_eval.Bool){
            trueBranch->evaluate();
        } else {
            break;
        }
    }
    if (next!= nullptr){ 
        next->evaluate(var_map);
    }
}
WhileNode::~WhileNode() {
    delete trueBranch;
}

//-----------------

PrintNode::PrintNode(ASTree* exp, SNode* next): SNode(exp, next) {
    //std::cout<< "Creating PrintNode.... ";
    //exp->print();
}
void PrintNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    value_bd ans = expression->evaluate();
    if (ans.type_tag == "bool"){
        if (ans.Bool) {
            std::cout << "true" << std::endl;
        } else {
            std::cout << "false" << std::endl;
        }
        
    } else {
        std::cout << ans.Double << std::endl;
    }

    if(next!= nullptr){
        next->evaluate(var_map);
    }
}
PrintNode::~PrintNode() {
    
}

//-----------------

IfNode::IfNode(ASTree* exp, SNode* next, STree* t, STree* f): SNode(exp, next), trueBranch(t), falseBranch(f) {
    //std::cout<< "Creating IfNode.... ";
    //exp->print();
}
void IfNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    value_bd exp_eval = expression->evaluate();
    if (exp_eval.type_tag != "bool") {
        throw EvaluationError("condition is not a bool.");
    }
    if (exp_eval.Bool){
        trueBranch->evaluate();
    } else {
        if (falseBranch!=nullptr) {
            falseBranch->evaluate();
        }
    }
    if (next!=nullptr){
        next->evaluate(var_map);
    }
}
IfNode::~IfNode() {
    delete trueBranch;
    delete falseBranch;
}

//-----------------

STree::STree(std::vector<token> tokens, std::unordered_map<std::string, value_bd>* var_map) {
    block = tokens;
    this->var_map = var_map;
    try {
        head = parse_block();
    } catch(const ParseError& e) {
        delete head;
        throw e;
    }
}

void STree::evaluate(){
    head->evaluate(var_map);
}

STree::~STree(){
    delete head;
}

SNode* STree::parse_block() {
    //BASE CASE PROBLEM AND REMOVE EXTRA PRINT STATEMENTS
    if(get_current_token().type == TokenType::END){
        return nullptr;
    }
    if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "if") {
        ASTree* exp = nullptr;
        STree* true_run = nullptr;
        STree* false_run = nullptr;
        // try {
            int temp_row = get_current_token().row;
            int temp_col = get_current_token().col;
            consume_token(); //eats up if
            std::vector<token> expression_tokens;
            std::vector<token> true_block_tokens;
            std::vector<token> false_block_tokens;
            while (get_current_token().type != TokenType::L_CURLY) {
                if(get_current_token().type == TokenType::END) {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
                temp_row = get_current_token().row;
                temp_col = get_current_token().col;
                expression_tokens.push_back(get_current_token());
                consume_token();
            }
            //Add end token to end of each expression that is being sent to ASTree
            token end_token{temp_row,temp_col+1,"END",TokenType::END};
            expression_tokens.push_back(end_token);
            exp = new ASTree(expression_tokens, var_map);
            consume_token(); //eats up left curly
            int open_braces = 0;
            while (open_braces>=0){
                if(get_current_token().type == TokenType::END) {
                    delete exp;
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
                if (get_current_token().type == TokenType::L_CURLY){
                    open_braces++;
                } else if (get_current_token().type == TokenType::R_CURLY){
                    open_braces--;
                }
                if (open_braces>=0){
                    true_block_tokens.push_back(get_current_token());
                    consume_token();
                }   
            }
            consume_token(); //eats up closing right curly
            true_block_tokens.push_back(end_token);
            try {
                true_run = new STree(true_block_tokens, var_map);
            } catch (const ParseError& e) {
                delete exp;
            }

            if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "else") {
                consume_token(); //eats up else
                if(get_current_token().type == TokenType::L_CURLY) { //else block
                    consume_token(); //eats up left curly
                    int open_braces = 0;
                    while (open_braces>=0){
                        if(get_current_token().type == TokenType::END) {
                            delete exp;
                            delete true_run;
                            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                        }
                        if (get_current_token().type == TokenType::L_CURLY){
                            open_braces++;
                        } else if (get_current_token().type == TokenType::R_CURLY){
                            open_braces--;
                        }
                        if (open_braces>=0){
                            false_block_tokens.push_back(get_current_token());
                            consume_token();
                        }   
                    }
                    false_block_tokens.push_back(end_token);
                    try {
                        false_run = new STree(false_block_tokens, var_map);
                    } catch (const ParseError& e) {
                        delete exp;
                        delete true_run;
                    }
                    consume_token(); //eats up closing right curly
                } else if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "if") { //else if block
                    std::vector<token> else_if;
                    else_if.push_back(get_current_token()); //add if to tokens
                    consume_token();
                    bool encountered = false;
                    int counter = 0;
                    while (true) {
                        if (!encountered && get_current_token().type == TokenType::END) {
                            delete exp;
                            delete true_run;
                            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                        }
                        if (encountered && counter == 0) {
                            if (get_current_token().text != "else") {
                                break;
                            }
                            encountered = false;
                        }
                        if (get_current_token().text == "}") {
                            else_if.push_back(get_current_token());
                            consume_token();
                            encountered = true;
                            --counter;
                        }
                        else {
                            if (get_current_token().text == "{") {
                                counter++;
                            }
                            else_if.push_back(get_current_token());
                            consume_token();
                        }
                    }
                    else_if.push_back(end_token);
                    try {
                        false_run = new STree(else_if, var_map);
                    } catch (const ParseError& e) {
                        delete exp;
                        delete true_run;
                    }
                } else {
                    delete exp;
                    delete true_run;
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token()); //else followed by weird shit
                }
            }
            return new IfNode(exp, parse_block(), true_run, false_run);
        // } catch (const ParseError& e) {
        //     delete exp;
        //     delete true_run;
        //     delete false_run;
        //     throw e;
        // }
    } else if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "while") {
        ASTree* exp = nullptr;
        STree* run = nullptr;
        // try {
            int temp_row = get_current_token().row;
            int temp_col = get_current_token().col;
            consume_token(); //eats up while
            std::vector<token> expression_tokens;
            std::vector<token> block_tokens;
            while (get_current_token().type != TokenType::L_CURLY) {
                if(get_current_token().type == TokenType::END) {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
                temp_row = get_current_token().row;
                temp_col = get_current_token().col;
                expression_tokens.push_back(get_current_token());
                consume_token();
            }
            //Add end token to end of each expression that is being sent to ASTree
            token end_token{temp_row,temp_col+1,"END",TokenType::END};
            expression_tokens.push_back(end_token);
            exp = new ASTree(expression_tokens, var_map);
            consume_token(); //eats up left curly
            int open_braces = 0;
            while (open_braces>=0){
                if(get_current_token().type == TokenType::END) {
                    delete exp;
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
                if (get_current_token().type == TokenType::L_CURLY){
                    open_braces++;
                } else if (get_current_token().type == TokenType::R_CURLY){
                    open_braces--;
                }
                if (open_braces>=0){
                    block_tokens.push_back(get_current_token());
                    consume_token();
                }   
            }
            block_tokens.push_back(end_token);
            try {
                run = new STree(block_tokens, var_map);
            } catch (const ParseError& e) {
                delete exp;
            }
            consume_token(); //eats up closing right curly
            return new WhileNode(exp, parse_block(), run);
        // } catch (const ParseError& e) {
        //     delete exp;
        //     delete run;
        //     throw e;
        // }
    } else if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "print") {
        ASTree* exp = nullptr;
        // try {
            int temp_row = get_current_token().row;
            int temp_col = get_current_token().col;
            consume_token(); //eats up print
            std::vector<token> expression_tokens;
            while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) { //check for seg fault
                temp_row = get_current_token().row;
                temp_col = get_current_token().col;
                expression_tokens.push_back(get_current_token());
                consume_token();
            }
            //Add end token to end of each expression that is being sent to ASTree
            token end_token{temp_row,temp_col+1,"END",TokenType::END};
            expression_tokens.push_back(end_token);
            exp = new ASTree(expression_tokens, var_map);
            return new PrintNode(exp, parse_block());
        // } catch (const ParseError& e) {
        //     delete exp;
        //     throw e;
        // }
    } else {
        ASTree* exp = nullptr;
        // try {
            std::vector<token> expression_tokens;
            int temp_row = get_current_token().row;
            int temp_col = get_current_token().col;
            while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) {
                temp_row = get_current_token().row;
                temp_col = get_current_token().col;
                expression_tokens.push_back(get_current_token());
                consume_token();
            }
            //Add end token to end of each expression that is being sent to ASTree
            token end_token{temp_row,temp_col+1,"END",TokenType::END};
            expression_tokens.push_back(end_token);
            exp = new ASTree(expression_tokens, var_map);
            return new ExpressionNode(exp, parse_block());
        // } catch (const ParseError& e) {
        //     delete exp;
        //     throw e;
        // }
    }
}