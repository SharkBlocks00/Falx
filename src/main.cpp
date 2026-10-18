#include <iostream>
#include "lexer/Lexer.h"

int main() {
    Lexer lexer{"let x = 5;", "Test"};

    const std::vector<Token> tokens{lexer.lex()};

    for (const auto& token: tokens) {
        std::cout << token.asString() << std::endl;
    }
}