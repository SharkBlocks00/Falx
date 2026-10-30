#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main() {
    try {
        Lexer lexer{"let x: int = 5; if (x==5) { output(5); } else { output(2);}", "Test"};

        const std::vector tokens{lexer.lex()};

        Parser parser{tokens};
        const std::vector statements{parser.parse()};

        Interpreter interpreter;
        interpreter.interpret(statements);
    } catch (const std::exception& e) {
        std::cerr << "Exception: [" << e.what() << "]\n";
        throw;
    }
}
