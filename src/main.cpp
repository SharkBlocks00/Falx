#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main() {
    Lexer lexer{"let x = 5; let b = 2;", "Test"};

    const std::vector tokens{lexer.lex()};

    Parser parser{tokens};
    const std::vector statements{parser.parse()};

    for (auto statement : statements) {
        std::cout << statement << std::endl;
    }
}
