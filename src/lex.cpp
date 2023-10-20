#include "lib/lex.h"

using namespace std;


void printTokens(const std::vector<token>& tokens) {
    
    for(const token& token : tokens) {
        if (token.text != "END") {
            std::cout << std::setw(4) << std::right << token.row << "   " << std::setw(2) << std::right << token.col << "  " << token.text << std::endl;
        }
    }
}

int main() {
    std::string line;
    int row = 1;
    int col = 1;
    while(std::getline(std::cin, line)) {
        std::cout << "INPUT: " << line << std::endl;
        try {
            auto tokens = tokenize(line, row, col);
            printTokens(tokens);
        } catch(const SyntaxError& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
        ++row;
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
