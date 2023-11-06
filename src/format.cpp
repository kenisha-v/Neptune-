#include "lib/ASTree.hpp"

//----------------------------------------------------------------------------

std::vector<token> find_condition(std::vector<token> input_tokens) {
    std::vector<token> cond;
    for (size_t i = 0; i < input_tokens.size(); ++i) {
        if (input_tokens[i].text == "{") {
            cond.push_back(input_tokens[input_tokens.size()-1]);
            break;
        }
        else if (input_tokens[i].text != "while" && input_tokens[i].text != "if" && input_tokens[i].text != "print") {
            cond.push_back(input_tokens[i]);
        }
    }
    return cond;
}

//----------------------------------------------------------------------------

int main() {
    std::string input;
    std::unordered_map<std::string, value_bd> Variable_Values; 
    std::unordered_map<std::string, value_bd> backup; //incase the parsing fails midway, we dont want to update variables
    ASTree* curr_tree = nullptr;
    size_t indent = 0;
    std::string output;
    int line = 0;
 

    while (std::getline(std::cin, input)){
        try {
            backup = Variable_Values;
            std::vector<token> input_tokens = tokenize(input);
            if (input_tokens.size() <=1) {
                ++line;
                continue;
            }
            if (input_tokens.at(0).text == "while") {
                for (size_t i = 0; i < indent; ++i) {
                    output += " ";
                }
                output += "while ";
                std::vector<token> while_cond = find_condition(input_tokens);
                curr_tree = new ASTree(while_cond, &Variable_Values);
                output += curr_tree->print_no_endl();
                output += " {\n";
                indent += 4;
            } else if (input_tokens.at(0).text == "if") {
                for (size_t i = 0; i < indent; ++i) {
                    output += " ";
                }
                output += "if ";
                std::vector<token> if_cond = find_condition(input_tokens);
                curr_tree = new ASTree(if_cond, &Variable_Values);
                output += curr_tree->print_no_endl();
                output += " {\n";
                indent += 4;
            } else if (input_tokens.at(0).text == "else") {
                for (size_t i = 0; i < indent; ++i) {
                    output += " ";
                }
                output += "else ";
                output += " {\n";
                indent += 4;
            } else if (input_tokens.at(0).text == "print") {
                for (size_t i = 0; i < indent; ++i) {
                    output += " ";
                }
                output += "print ";
                if (input_tokens.size()<=3) {
                    for (size_t i = 1; i < input_tokens.size()-1; ++i) {
                        output += input_tokens[i].text;
                        if (i != input_tokens.size()-2) {
                            output += " ";
                        }
                    }
                    output += "\n";
                }
                else {
                        for (size_t i = 0; i < indent; ++i) {
                        output += " ";
                    }
                    std::vector<token> print_cond = find_condition(input_tokens);
                    curr_tree = new ASTree(print_cond, &Variable_Values);
                    output += curr_tree->print_no_endl() + "\n";
                }
            } else if (input_tokens.at(0).text == "}") {
                indent -= 4;
                for (size_t i = 0; i < indent; ++i) {
                    output += " ";
                }
                output += "}\n";
            } else {
                curr_tree = new ASTree(input_tokens, &Variable_Values);
                for (size_t i = 0; i < indent; ++i) {
                    output += " ";
                }
                output += curr_tree->print_no_endl() + "\n";
            }
            ++ line;
            // curr_tree = new ASTree(input_tokens, &Variable_Values);
            // curr_tree->print();
            //check statement for double or bool return type
            // if(curr_tree->evaluate().type_tag == "bool") {
            //     if(curr_tree->evaluate().Bool) {
            //         std::cout << "true" << std::endl;
            //     } else {`
            //         std::cout << "false" << std::endl;
            //     }
            // } else {
            //     std::cout << curr_tree->evaluate().Double << std::endl;
            // }
        } catch (const SyntaxError& e) {
            std::string message = "Syntax error on line " + std::to_string(e.get_row()+line) + " column " + std::to_string(e.get_col()) + ".";
            std::cout << message << std::endl;
            return 1;
            
        } catch (const ParseError& e) {
            Variable_Values = backup; //not needed here, but just to be safe. 
            std::string message = "Unexpected token at line " + std::to_string(e.get_row()+line) + " column " + std::to_string(e.get_col()) + ": " + e.get_error_token().text;
            std::cout << message << std::endl;
            return 2;
        }
        delete curr_tree;
        curr_tree = nullptr;
    }
    
    std::cout << output ;
    
} 
