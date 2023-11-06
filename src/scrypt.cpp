#include "lib/STree.hpp"

int main() {
    std::string input;
    char ch;
    STree* my_tree = nullptr;
    std::unordered_map<std::string, value_bd> var_map;

    while (std::cin.get(ch)) {
        input += ch;
    }
    std::vector<token> tokens = tokenize(input);
    my_tree = new STree(tokens, &var_map);
    my_tree->evaluate();
    
    return 0;
}