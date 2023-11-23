#include "STree.hpp"

//-----------------

SNode::SNode(EXP* exp, SNode* next): expression(exp), next(next) {}
SNode::~SNode() {
    delete expression;
    delete next;
}
void SNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    (void)var_map;
    return;
}

//-----------------

ExpressionNode::ExpressionNode(EXP* exp, SNode* next): SNode(exp, next) {
    
}
void ExpressionNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    expression->expression->evaluate();
    if (next!= nullptr){
        next->evaluate(var_map);
    }
}
void ExpressionNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    if (expression->type == "expression"){
        std::cout << expression->expression->print_no_endl();
    } else if (expression->type == "funciton"){
        expression->function->print();
    } else if (expression->type == "function_assigner"){
        std::cout << expression->expression->print_no_endl() << " = ";
        expression->function->print();
    } else {
        std::cout << "SOMTHING BAD IS HAPPENING \n NOT SUPPOSED TO REACH HERE" << std::endl;
    }

    std::cout << ";\n";
    
    if(next != nullptr) {
        next->print(tab);
    }
}
ExpressionNode::~ExpressionNode() {
    
}

//-----------------

WhileNode::WhileNode(EXP* exp, SNode* next, STree* t): SNode(exp, next), trueBranch(t) {
    
}
void WhileNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    value_bd exp_eval;
    while (true){
        exp_eval = expression->expression->evaluate();
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
    std::cout << expression->expression->print_no_endl(); 
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

PrintNode::PrintNode(EXP* exp, SNode* next): SNode(exp, next) {
    
}
void PrintNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    value_bd ans = expression->expression->evaluate();
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
    if(expression->type == "expression"){
        std::cout << expression->expression->print_no_endl();
    } else{
        expression->function->print();
    }
    std::cout << ";\n";
    if(next != nullptr) {
        next->print(tab);
    }
}
PrintNode::~PrintNode() {
    
}

//-----------------

IfNode::IfNode(EXP* exp, SNode* next, STree* t, STree* f): SNode(exp, next), trueBranch(t), falseBranch(f) {
    
}
void IfNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    value_bd exp_eval = expression->expression->evaluate();
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
    std::cout << expression->expression->print_no_endl();
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

FuncNode::FuncNode(std::unordered_map<std::string, value_bd>* local_v, SNode* next, STree* code, std::vector<std::string> p, std::string name): 
    SNode(nullptr, next),
    f_name(name),
    local_var_map(local_v),
    parameters(p),
    code(code) {}
void FuncNode::evaluate(std::unordered_map<std::string, value_bd>* var_map) {
    std::unordered_map<std::string, value_bd> var_map_copy = (*var_map); //will this work
    local_var_map = &var_map_copy;
    if(next){
        next->evaluate(var_map);
    }
}
// void FuncNode::call(std::vector<token> arguments){
//     (void)arguments;
//     return; 
// }
void FuncNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "def "; 
    std::cout << f_name << "(";
    for(size_t i=0; i<parameters.size(); i++){
        if(i==parameters.size()-1){
            std::cout << parameters[i];
        }else{
            std::cout << parameters[i] << ", ";
        }
    }
    std::cout << ")";
    std::cout << " {" << std::endl;
    tab += 4;
    if (code){
        code->print(tab);
    }
    tab -= 4;
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "}" << std::endl;
    if(next != nullptr) {
        next->print(tab);
    }
}
FuncNode::~FuncNode() {
    delete code;
    delete local_var_map;
}

//-----------------

ReturnNode::ReturnNode(EXP* exp, SNode* next): SNode(exp,next){}

void ReturnNode::evaluate(std::unordered_map<std::string, value_bd>* var_map){
    expression->expression->evaluate();
    (void)var_map;
    //does not call next, as we dont need to evaluate after return.
    }

// value_bd ReturnNode::call(std::unordered_map<std::string, value_bd>* var_map){
//     expression->evaluate();
//     //does not call next, as we dont need to evaluate after return.
//     }

void ReturnNode::print(int tab){
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "return";
    if (expression){
        std::cout << " " << expression->expression->print_no_endl();
    }
    std::cout << ";\n";
    if(next != nullptr) {
        next->print(tab);
    }
}

ReturnNode::~ReturnNode(){
    
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
        EXP* exp = nullptr;
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
        exp = new EXP(new ASTree(expression_tokens, var_map));
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
        EXP* exp = nullptr;
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
        exp = new EXP(new ASTree(expression_tokens, var_map));
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
        EXP* exp = nullptr;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        bool semi_colon = false;
        consume_token(); //consume print
        if(get_current_token().type == TokenType::VARIABLES && block[current_token_index+1].type == TokenType::LEFT_PAREN) { //function
            std::vector<ASTree*> arg;
            std::string name = get_current_token().text;
            consume_token(); consume_token(); //consume func_name and left paren
            std::vector<token> expression_tokens;
            while (get_current_token().type != TokenType::RIGHT_PAREN) {
                if (get_current_token().type == TokenType::COMMA){
                    if (expression_tokens.size()==0){
                        throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                    }
                    token end_token{temp_row,temp_col+1,"END",TokenType::END};
                    expression_tokens.push_back(end_token);
                    ASTree* single_argument = new ASTree(expression_tokens, var_map);
                    arg.push_back(single_argument);
                    expression_tokens.clear();
                    consume_token();
                }   else {
                    expression_tokens.push_back(get_current_token());
                    consume_token();
                }

            }
            if (expression_tokens.size()!=0){
                token end_token{temp_row,temp_col+1,"END",TokenType::END};
                expression_tokens.push_back(end_token);
                ASTree* single_argument = new ASTree(expression_tokens, var_map);
                arg.push_back(single_argument);
                expression_tokens.clear();
            }
            consume_token(); // consuming right paren
            if (get_current_token().text == ";") {
                semi_colon = true;
                consume_token();
            }
            exp = new EXP(new function_call(name, arg));
        } else { //expression
            std::vector<token> expression_tokens;
            //store the expression condition to give to ASTree
            while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) {
                if (get_current_token().text == ";") {
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
            token end_token{temp_row,temp_col+1,"END",TokenType::END};
            expression_tokens.push_back(end_token);
            exp = new EXP(new ASTree(expression_tokens, var_map));
        }
        if (semi_colon == false) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }
        return new PrintNode(exp, parse_block());
    } 
    


    //FUNCTION STATEMENT
    else if(get_current_token().text == "def" && block[current_token_index+1].text != "=") { //def is a keyword
        STree* code = nullptr;
        std::vector<std::string> params;
        std::vector<token> block_tokens;
        std::vector<token> return_tokens;
        std::unordered_map<std::string, value_bd>* local_var_map = new std::unordered_map<std::string, value_bd>; // does this work?
        consume_token(); //consume def
        if (get_current_token().type != TokenType::VARIABLES) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }
        std::string func_name = get_current_token().text;
        consume_token(); //consume function name
        if (get_current_token().type != TokenType::LEFT_PAREN) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }
        consume_token(); //consume (

        bool curr_comma = true;
        while (get_current_token().type != TokenType::RIGHT_PAREN) {
            if(curr_comma){
                if (get_current_token().type == TokenType::VARIABLES){
                    params.push_back(get_current_token().text);
                    curr_comma=false;
                } else {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
            } else {
                if (get_current_token().type == TokenType::COMMA){
                    curr_comma = true;
                } else {
                    throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                }
            }
            consume_token();
        }
        consume_token(); //consume )
        //curly brace counter make block for code and give to STree with new scope (new var_map)
        consume_token(); //consume left curly
        int open_braces = 0; //keep track of curly braces
        //take all tokens inside the braces to create a new tree using recursion
        while (open_braces>=0){
            if(get_current_token().type == TokenType::END) {
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
        if (block_tokens.size() != 0){
            token end_token{get_current_token().row,get_current_token().col,"END",TokenType::END};
            block_tokens.push_back(end_token);
            code = new STree(block_tokens, local_var_map); //does this work
        }
        consume_token(); //consume }
        
        return new FuncNode(local_var_map, parse_block(), code, params, func_name);;
    }



    //RETURN STATEMENT
    else if (get_current_token().text == "return") {
        EXP* exp = nullptr;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        consume_token(); //consume return
        std::vector<token> return_value;
        bool semi_colon = false;
        while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) {
            if (get_current_token().text == ";") {
                semi_colon = true;
                consume_token();
                break;
            }
            temp_row = get_current_token().row;
            temp_col = get_current_token().col;
            return_value.push_back(get_current_token());
            consume_token();
        }

        if (semi_colon == false) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }

        //Add end token to end of each expression that is being sent to ASTree
        if (return_value.size() != 0){
            token end_token{temp_row,temp_col+1,"END",TokenType::END};
            return_value.push_back(end_token);  
            exp = new EXP(new ASTree(return_value, var_map));
        }
        // if an empty list, exp will be passed on as nullptr
        return new ReturnNode(exp, parse_block());
    }


    //EXPRESSION STATEMENT
    else {
        EXP* exp = nullptr;
        function_call* fc = nullptr;
        std::vector<token> expression_tokens;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        bool semi_colon = false;
        //store entire line to give to ASTree
        while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) {
            if (get_current_token().text == ";") {
                semi_colon = true;
                consume_token();
                break;
            }

            if(get_current_token().type == TokenType::VARIABLES && block[current_token_index+1].type == TokenType::LEFT_PAREN) { //function
                std::vector<ASTree*> arg;
                std::string name = get_current_token().text;
                consume_token(); consume_token(); //consume func_name and left paren
                std::vector<token> expression_tokens;
                while (get_current_token().type != TokenType::RIGHT_PAREN) {
                    if (get_current_token().type == TokenType::COMMA){
                        if (expression_tokens.size()==0){
                            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
                        }
                        token end_token{temp_row,temp_col+1,"END",TokenType::END};
                        expression_tokens.push_back(end_token);
                        ASTree* single_argument = new ASTree(expression_tokens, var_map);
                        arg.push_back(single_argument);
                        expression_tokens.clear();
                        consume_token();
                    }   else {
                        expression_tokens.push_back(get_current_token());
                        consume_token();
                    }

                }
                if (expression_tokens.size()!=0){
                    token end_token{temp_row,temp_col+1,"END",TokenType::END};
                    expression_tokens.push_back(end_token);
                    ASTree* single_argument = new ASTree(expression_tokens, var_map);
                    arg.push_back(single_argument);
                    expression_tokens.clear();
                }
                consume_token(); // consuming right paren
                fc = new function_call(name, arg);
            } else {
                temp_row = get_current_token().row;
                temp_col = get_current_token().col;
                expression_tokens.push_back(get_current_token());
                consume_token();
            }
        }
        
        if (semi_colon == false) {
            throw ParseError(get_current_token().row, get_current_token().col+1, get_current_token());
        }

        if (fc==nullptr){//only expression
            {//add end token to end of each expression that is being sent to ASTree
            token end_token{temp_row,temp_col+1,"END",TokenType::END};
            expression_tokens.push_back(end_token);
            }
            exp = new EXP(new ASTree(expression_tokens, var_map));
        } else if (expression_tokens.size() == 0){ //only function
            exp = new EXP(fc);
        } else { // expression with function
            exp = new EXP(expression_tokens, fc);
        }

        return new ExpressionNode(exp, parse_block());
    }
}