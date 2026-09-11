import sys
import traceback

from src.ArgumentHandler import ArgumentHandler
from src.ast.Statement import Statement
from src.lexer.Lexer import Lexer
from src.parser.Parser import Parser
from src.runtime.Interpreter import Interpreter
from src.tokens.Token import Token


def main():

    argumentHandler: ArgumentHandler = ArgumentHandler(sys.argv)
    obj: object = None
    try:
        obj = argumentHandler.handleArguments()
    except Exception as e:
        print(e)
    if obj is None: return

    source: str = obj.__str__()
    lexer: Lexer = Lexer(source)
    try:
        tokens: list[Token] = lexer.lex()

        parser: Parser = Parser(tokens)

        statements: list[Statement] = parser.parse()

        interpreter: Interpreter = Interpreter()
        interpreter.interpret(statements)
    except Exception:
        traceback.print_exc()

if __name__ == "__main__":
    main()

