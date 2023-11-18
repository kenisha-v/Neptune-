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
    
}
void ExpressionNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    expression->evaluate();
    if (next!= nullptr){
        next->evaluate(var_map);
    }
}
void ExpressionNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    expression->print();
    if(next != nullptr) {
        next->print(tab);
    }
}
ExpressionNode::~ExpressionNode() {
    
}

//-----------------

WhileNode::WhileNode(ASTree* exp, SNode* next, STree* t): SNode(exp, next), trueBranch(t) {
    
}
void WhileNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    value_bd exp_eval;
    while (true){
        exp_eval = expression->evaluate();
        if (exp_eval.type_tag != "bool") {
            throw EvaluationError("condition is not a bool.");
        }
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
void WhileNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "while "; 
    std::cout << expression->print_no_endl(); 
    std::cout << " {" << std::endl;
    tab += 4;
    trueBranch->print(tab);
    std::cout << "}" << std::endl;
    tab -= 4;
    if(next != nullptr) {
        next->print(tab);
    }
}
WhileNode::~WhileNode() {
    delete trueBranch;
}

//-----------------

PrintNode::PrintNode(ASTree* exp, SNode* next): SNode(exp, next) {
    
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
void PrintNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "print ";
    expression->print();
    if(next != nullptr) {
        next->print(tab);
    }
}
PrintNode::~PrintNode() {
    
}

//-----------------

IfNode::IfNode(ASTree* exp, SNode* next, STree* t, STree* f): SNode(exp, next), trueBranch(t), falseBranch(f) {
    
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
void IfNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "if ";
    std::cout << expression->print_no_endl();
    std::cout << " {" <<std::endl;
    tab += 4;
    trueBranch->print(tab);
    tab -= 4;
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "}"<<std::endl;
    if (falseBranch != nullptr) {
        for (int i = 0; i < tab; ++i) {
            std::cout << " ";
        }
        std::cout << "else ";
        std::cout << "{" << std::endl;
        tab += 4;
        falseBranch->print(tab);
        tab -= 4;
        for (int i = 0; i < tab; ++i) {
            std::cout << " ";
        }
        std::cout << "}" << std::endl;
    }
    if(next != nullptr) {
        next->print(tab);
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

SNode* STree::get_head() {
    return head;
}

void STree::print(int tab) {
    head->print(tab);
}

STree::~STree(){
    delete head;
}

SNode* STree::parse_block() {



    //BASE CASE
    if(get_current_token().type == TokenType::END){
        return nullptr;
    }



    //IF STATEMENT 
    if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "if") {
        ASTree* exp = nullptr;
        STree* true_run = nullptr;
        STree* false_run = nullptr;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        consume_token(); //consume if
        std::vector<token> expression_tokens;
        std::vector<token> true_block_tokens;
        std::vector<token> false_block_tokens;
        //store the expression condition to give to ASTree
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
        consume_token(); //consume left curly
        int open_braces = 0; //keep track of curly braces
        //take all tokens inside the braces to create a new tree using recursion
        while (open_braces>=0){
            temp_row = get_current_token().row;
            temp_col = get_current_token().col;
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
        consume_token(); //consume closing right curly
        true_block_tokens.push_back(end_token);
        try {
            true_run = new STree(true_block_tokens, var_map);
        } catch (const ParseError& e) {
            delete exp;
        }
        //statement if is followed by an else
        if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "else") {
            consume_token(); //consume else
            //start of else block
            if(get_current_token().type == TokenType::L_CURLY) {
                consume_token(); //consume left curly
                int open_braces = 0; //keep track of curly braces
                //take all tokens inside the braces to create a new tree using recursion
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
                consume_token(); //consume closing right curly
            }
            //start of else if block 
            else if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "if") {
                bool block_end = false;
                int brace_count = 0; //keep track of curly braces
                false_block_tokens.push_back(get_current_token()); //add if to tokens
                consume_token();
                //add the entirety of the else if till the end of the last connected else to create another tree using recursion
                while (true) {
                    if (!block_end && get_current_token().type == TokenType::END) {
                        delete exp;
                        delete true_run;
                        throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                    }
                    if (block_end && brace_count == 0) {
                        if (get_current_token().text != "else") {
                            break;
                        }
                        block_end = false;
                    }
                    if (get_current_token().text == "}") {
                        false_block_tokens.push_back(get_current_token());
                        consume_token();
                        --brace_count;
                        if (brace_count <= 0) {
                            block_end = true;
                        }
                    }
                    else {
                        if (get_current_token().text == "{") {
                            brace_count++;
                        }
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
            } 
            //in case if is followed by else but not a curly brace of another if statement
            else {
                delete exp;
                delete true_run;
                throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
            }
        }
        return new IfNode(exp, parse_block(), true_run, false_run);
    } 



    //WHILE STATEMENT
    else if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "while") {
        ASTree* exp = nullptr;
        STree* run = nullptr;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        consume_token(); //consume while
        std::vector<token> expression_tokens;
        std::vector<token> block_tokens;
        //store the expression condition to give to ASTree
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
        consume_token(); //consume left curly
        int open_braces = 0; //keep track of curly braces
        //take all tokens inside the braces to create a new tree using recursion
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
        consume_token(); //consume closing right curly
        return new WhileNode(exp, parse_block(), run);
    } 



    //PRINT STATEMENT
    else if(get_current_token().type == TokenType::STATEMENT && get_current_token().text == "print") {
        ASTree* exp = nullptr;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        consume_token(); //consume print
        std::vector<token> expression_tokens;
        bool semi_colon = false;
        //store the expression condition to give to ASTree
        while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) {
            if (get_current_token().text == ";") {
                //std::cout << "print statement last node is ; \n" ;
                semi_colon = true;
                consume_token();
                break;
            }
            temp_row = get_current_token().row;
            temp_col = get_current_token().col;
            expression_tokens.push_back(get_current_token());
            consume_token();
        }
        //Add end token to end of each expression that is being sent to ASTree
        if (semi_colon == false) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }
        token end_token{temp_row,temp_col+1,"END",TokenType::END};
        expression_tokens.push_back(end_token);
        exp = new ASTree(expression_tokens, var_map);
        return new PrintNode(exp, parse_block());
    } 
    


    //EXPRESSION STATEMENT
    else {
        ASTree* exp = nullptr;
        std::vector<token> expression_tokens;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        bool semi_colon = false;
        //store entire line to give to ASTree
        while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) {
            if (get_current_token().text == ";") {
                //std::cout << "expression statement last node is ; \n" ;
                semi_colon = true;
                consume_token();
                break;
            }
            temp_row = get_current_token().row;
            temp_col = get_current_token().col;
            expression_tokens.push_back(get_current_token());
            consume_token();
        }
        if (semi_colon == false) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }
        //Add end token to end of each expression that is being sent to ASTree
        token end_token{temp_row,temp_col+1,"END",TokenType::END};
        expression_tokens.push_back(end_token);
        exp = new ASTree(expression_tokens, var_map);
        return new ExpressionNode(exp, parse_block());
    }
}