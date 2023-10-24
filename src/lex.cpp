#include "lib/lex.h"
#include "lib/errors.h"

using namespace std;


void printTokens(const std::vector<token>& tokens) {
    
    for(const token& token : tokens) {
        std::cout << std::setw(4) << std::right << token.row << "   " << std::setw(2) << std::right << token.col << "  " << token.text << std::endl;
    }
}

int main() {
    std::string input;
    char ch;

    while (std::cin.get(ch)) {
        input += ch;
    }

    // std::string line;
    // std::string input;
    // while(std::getline(std::cin, line)) {
    //     input += line;
    //     input += "\n";
    // }
    try {
        auto tokens = tokenize(input);
        printTokens(tokens);
    } catch(const SyntaxError& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    

    //std::string input;
    //char ch;

    // while (std::cin.get(ch)) {
    //     input += ch;
    // }

    // try {
    //     auto tokens = tokenize(input);
    //     printTokens(tokens);
    // } catch(const SyntaxError& e) {
    //     std::cout << e.what() << std::endl;
    //     return 1;
    // }
    
    return 0;
}
