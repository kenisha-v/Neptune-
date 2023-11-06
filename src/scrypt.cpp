#include "lib/STree.hpp"

int main() {
    std::string input;
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
        std::cout << e.what() << std::endl;
        return 1;
    } catch (const ParseError& e) {
        std::cout << e.what() << std::endl;
        return 2;
    } catch (const EvaluationError& e) {
        std::cout << e.what() << std::endl;
        return 3;
    }

    
    
    
    return 0;
}