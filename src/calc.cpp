#include "lib/ASTree.hpp"

int main() {
    std::string input;
    std::unordered_map<std::string, value_bd> Variable_Values; 
    std::unordered_map<std::string, value_bd> backup; //incase the parsing fails midway, we dont want to update variables
    ASTree* curr_tree = nullptr;

    while (true){
        try {
            std::getline(std::cin, input);
            if (std::cin.eof()) {
                break;
            }
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
        } catch (const ParseError& e) {
            Variable_Values = backup; //not needed here, but just to be safe.
            std::cout << e.what() << std::endl;
            std::string temp = e.what();
            if(temp == "Unexpected token at line 1 column 15: |" || temp == "Unexpected token at line 1 column 21: |"){
                cout << "testtttt :" << input << endl;
            }
        } catch (const EvaluationError& e) {
            Variable_Values = backup;
            std::cout << e.what() << std::endl;
        }
        delete curr_tree;
        curr_tree = nullptr;
    }
} 
