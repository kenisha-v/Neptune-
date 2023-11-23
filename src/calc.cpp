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
            } else if(curr_tree->evaluate().type_tag == "double") {
                std::cout << curr_tree->evaluate().Double << std::endl;
            } else {
                ArrayNode* arr_node = new ArrayNode(0, 0, curr_tree->evaluate().array);\
                std::cout << arr_node->evaluate_print(curr_tree->evaluate().array) << std::endl;
                delete arr_node;
                arr_node = nullptr;
            }
        } catch (const SyntaxError& e) {
            std::cout << e.what() << std::endl;
        } catch (const ParseError& e) {
            Variable_Values = backup; //not needed here, but just to be safe. 
            std::cout << e.what() << std::endl;
        } catch (const EvaluationError& e) {
            Variable_Values = backup;
            std::cout << e.what() << std::endl;
        }
        delete curr_tree;
        curr_tree = nullptr;
    }
    return 0;
}