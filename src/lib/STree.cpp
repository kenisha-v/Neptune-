#include "STree.hpp"

//-----------------

SNode::SNode(ASTree* exp, SNode* next): expression(exp), next(next) {}
SNode::~SNode() {
    delete expression;
    delete next;
}
void SNode::evaluate(std::unordered_map<std::string, value_bd>* var_map, std::unordered_map<std::string, FuncNode*>* func_map) {
    (void)var_map;
    (void)func_map;
    return;
}

//-----------------

ExpressionNode::ExpressionNode(ASTree* exp, SNode* next): SNode(exp, next) {
    
}
void ExpressionNode::evaluate(std::unordered_map<std::string, value_bd>* var_map, std::unordered_map<std::string, FuncNode*>* func_map) {
    expression->evaluate();
    if (next!= nullptr){
        next->evaluate(var_map, func_map);
    }
}
void ExpressionNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << expression->print_no_endl();
    std::cout << ";\n";
    
    if(next != nullptr) {
        next->print(tab);
    }
}
ExpressionNode::~ExpressionNode() {
    
}

//-----------------

WhileNode::WhileNode(ASTree* exp, SNode* next, STree* t): SNode(exp, next), trueBranch(t) {
    
}
void WhileNode::evaluate(std::unordered_map<std::string, value_bd>* var_map, std::unordered_map<std::string, FuncNode*>* func_map) {
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
        next->evaluate(var_map, func_map);
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
void PrintNode::evaluate(std::unordered_map<std::string, value_bd>* var_map, std::unordered_map<std::string, FuncNode*>* func_map) {
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
        next->evaluate(var_map, func_map);
    }
}
void PrintNode::print(int tab) {
    for (int i = 0; i < tab; ++i) {
        std::cout << " ";
    }
    std::cout << "print ";
    std::cout << expression->print_no_endl();
    std::cout << ";\n";
    if(next != nullptr) {
        next->print(tab);
    }
}
PrintNode::~PrintNode() {
    
}

//-----------------

IfNode::IfNode(ASTree* exp, SNode* next, STree* t, STree* f): SNode(exp, next), trueBranch(t), falseBranch(f) {
    
}
void IfNode::evaluate(std::unordered_map<std::string, value_bd>* var_map, std::unordered_map<std::string, FuncNode*>* func_map) {
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
        next->evaluate(var_map, func_map);
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

FuncNode::FuncNode(std::unordered_map<std::string, value_bd>* local_v, std::unordered_map<std::string, FuncNode*>* local_f, SNode* next, STree* code): 
    SNode(nullptr, next), 
    local_var_map(local_v), 
    local_func_map(local_f), 
    code(code) {}
void FuncNode::evaluate(std::unordered_map<std::string, value_bd>* var_map, std::unordered_map<std::string, FuncNode*>* func_map) {
    std::unordered_map<std::string, value_bd> var_map_copy = (*var_map); //will this work
    std::unordered_map<std::string, FuncNode*> func_map_copy = (*func_map); //will this work
    local_var_map = &var_map_copy;
    local_func_map = &func_map_copy;
}
void FuncNode::print(int tab) {
    (void)tab;
}
FuncNode::~FuncNode() {
    delete code;
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
    head->evaluate(var_map, func_map);
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
        if (semi_colon == false) {
            throw ParseError(get_current_token().row, get_current_token().col, get_current_token());
        }
        expression_tokens.push_back(end_token);
        exp = new ASTree(expression_tokens, var_map);
        return new PrintNode(exp, parse_block());
    } 
    


    //FUNCTION STATEMENT
    else if(get_current_token().text == "def" && block[current_token_index+1].text != "=") { //def is a keyword
        STree* code = nullptr;
        std::vector<std::string> params;
        std::vector<token> block_tokens;
        std::unordered_map<std::string, value_bd>* local_var_map = new std::unordered_map<std::string, value_bd>; // does this work?
        std::unordered_map<std::string, FuncNode*>* local_func_map = new std::unordered_map<std::string, FuncNode*>; // does this work?
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
        while (get_current_token().type != TokenType::RIGHT_PAREN) {
            if(get_current_token().type != TokenType::COMMA){
                params.push_back(get_current_token().text);
            } 
            consume_token();
        }
        consume_token(); //consume )
        //curly brace counter make block for code and give to STree with new scope (new var_map and new func_map)
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
        token end_token{get_current_token().row,get_current_token().col,"END",TokenType::END};
        block_tokens.push_back(end_token);
        code = new STree(block_tokens, local_var_map); //does this work
        consume_token(); //consume }
        FuncNode* fnode = new FuncNode(local_var_map, local_func_map, parse_block(), code);
        (*func_map)[func_name] = fnode;
        return fnode;
    }



    //EXPRESSION STATEMENT
    else {
        ASTree* exp = nullptr;
        std::vector<token> expression_tokens;
        int temp_row = get_current_token().row;
        int temp_col = get_current_token().col;
        bool semi_colon = false;
        bool hasFunc = false;
        //store entire line to give to ASTree
        while (get_current_token().row == temp_row && get_current_token().type != TokenType::END) {
            if (get_current_token().text == ";") {
                semi_colon = true;
                consume_token();
                break;
            }
            if((*func_map).find(get_current_token().text) != (*func_map).end()){
                hasFunc = true;
                break;
            }
            temp_row = get_current_token().row;
            temp_col = get_current_token().col;
            expression_tokens.push_back(get_current_token());
            consume_token();
        }
        if (!semi_colon) {
            throw ParseError(get_current_token().row, get_current_token().col+1, get_current_token());
        }
        if (hasFunc) {
            name = curr_token... + consume token
            check if curr token now is a "(" + if yes consume that | if no - throw error..
            string parameters - keep adding and cosuming tokens untill ")"
            consume ")"
            check if line ends, (via curr_token.row)

            vector params <expressions (expressions: input type: numbers are a form of expression)> ===== string parameters.split(,):

            params = (4, 7+0, 8*3)
            send each expression in params to calc, and get a value, replace that value with the current expression:
            (if it is just a number, calc will return number)
            parems = (4,7,24)

            check if length of paren matches with the number of inputs required by the function: name stored in a variable line 1:

            if it matches, update function_local_var_map: with the inputs (probably using index numbers):
            ex. foo(w,x,y):
            (w=4), (x=7), (y=24)

            once varmap is updated,
            call func_code.evaluate() -> this will update local func variables with whatever the function was suposed to do:

            call func_return(which is an astree) with its evaluate -- this gives us a return value bd.
            make token of that return value bd and add to expression_tokens,
            
            let the code below continue, 

        }
        //Add end token to end of each expression that is being sent to ASTree
        token end_token{temp_row,temp_col+1,"END",TokenType::END};
        expression_tokens.push_back(end_token);
        exp = new ASTree(expression_tokens, var_map);
        return new ExpressionNode(exp, parse_block());
    }
}