#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main() {
    Lexer lexer{"let in: bool = false; if (5 == 5) {in = true; } ", "Test"};

    const std::vector tokens{lexer.lex()};

    Parser parser{tokens};
    const std::vector statements{parser.parse()};

    Interpreter interpreter;
    interpreter.interpret(statements);
}
