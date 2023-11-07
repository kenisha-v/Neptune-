#include "lib/ASTree.hpp"
#include "lib/STree.hpp"

int main() {
    std::string input;
    std::string error;
    char ch;
    std::unordered_map<std::string, value_bd> var_map;
    int tab_level = 0;

    while (std::cin.get(ch)) {
        input += ch;
    }

    try{
        std::vector<token> tokens = tokenize(input);
        STree my_tree(tokens, &var_map);
        my_tree.print(tab_level);
    } catch (const SyntaxError& e) {
        std::cout << e.what() << std::endl;
        return 1;
    } catch (const ParseError& e) {
        error = e.what();
        std::cout << e.what() << std::endl;
        return 2;
    }
    return 0;
}
