#include "lib/STree.hpp"

int main() {
    std::string input;
    std::string error;
    char ch;
    STree* my_tree = nullptr;
    std::unordered_map<std::string, value_bd> var_map;

    while (std::cin.get(ch)) {
        input += ch;
    }

    try{
        std::vector<token> tokens = tokenize(input);
        my_tree = new STree(tokens, &var_map);
        my_tree->evaluate();
    } catch (const SyntaxError& e) {
        error = e.what();
        std::cout << e.what() << std::endl;
        return 1;
    } catch (const ParseError& e) {
        error = e.what();
        std::cout << e.what() << std::endl;
        if(error == "Unexpected token at line 22 column 1: END") {
            std::cout << input << std::endl;
        }
        if(error == "Unexpected token at line 4 column 13: END") {
            std::cout << input << std::endl;
        }
        if(error == "Unexpected token at line 5 column 8: END") {
            std::cout << input << std::endl;
        }
        return 2;
    } catch (const EvaluationError& e) {
        error = e.what();
        std::cout << e.what() << std::endl;
        return 3;
    }

    
    
    
    return 0;
}