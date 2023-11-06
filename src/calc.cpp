#include "lib/ASTree.hpp"

int main() {
    std::string input;
    std::unordered_map<std::string, value_bd> Variable_Values; 
    std::unordered_map<std::string, value_bd> backup; //incase the parsing fails midway, we dont want to update variables
    ASTree* curr_tree = nullptr;

    while (std::getline(std::cin, input)){
        try {
            backup = Variable_Values;
            std::vector<token> input_tokens = tokenize(input);
            curr_tree = new ASTree(input_tokens, &Variable_Values);
            curr_tree->print();
            //check statement for double or bool return type
            if(curr_tree->evaluate().type_tag == "bool") {
                if(curr_tree->evaluate().Bool) {
                    std::cout << "true" << std::endl;
                } else {
                    std::cout << "false" << std::endl;
                }
            } else {
                std::cout << curr_tree->evaluate().Double << std::endl;
            }
        } catch (const SyntaxError& e) {
            std::cout << e.what() << std::endl;
            return 1;
        } catch (const ParseError& e) {
            Variable_Values = backup; //not needed here, but just to be safe.
            std::cout << e.what() << std::endl;
            return 2;
        } catch (const EvaluationError& e) {
            Variable_Values = backup;
            std::cout << e.what() << std::endl;
            return 3;
        }
        delete curr_tree;
        curr_tree = nullptr;
    }
    return 0;
}