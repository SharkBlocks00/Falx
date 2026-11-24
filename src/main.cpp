#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "types/TypeChecker.h"

int main() {
    try {
        Lexer lexer{"output(typeof(true)); let x: int = typeof(true); output(typeof(x));", "Test"};

        const std::vector tokens{lexer.lex()};

        Parser parser{tokens};
        const std::vector statements{parser.parse()};

        TypeChecker typeChecker{};
        typeChecker.check(statements);

        Interpreter interpreter;
        interpreter.interpret(statements);
    } catch (const std::exception& e) {
        std::cerr << "Exception: [" << e.what() << "]\n";
        throw;
    }
}
