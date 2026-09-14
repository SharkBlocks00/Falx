import traceback
from pathlib import Path
import sys

from src.ArgumentHandler import ArgumentHandler
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.diagnostics.DiagnosticFactory import createDiagnostic
from src.diagnostics.DiagnosticPrinter import DiagnosticPrinter
from src.diagnostics.exceptions.FalxException import FalxException
from src.lexer.Lexer import Lexer
from src.parser.Parser import Parser
from src.runtime.Interpreter import Interpreter
from src.tokens.Token import Token


def main():
    argumentHandler: ArgumentHandler = ArgumentHandler(sys.argv)

    try:
        scriptPath: Path | None = argumentHandler.handleArguments()
    except Exception as e:
        print(e, file=sys.stderr)
        return

    if scriptPath is None:
        return

    source: str = scriptPath.read_text(encoding="utf-8")

    try:
        lexer: Lexer = Lexer(source)
        tokens: list[Token] = lexer.lex()

        parser: Parser = Parser(tokens)
        statements: list[Statement] = parser.parse()

        interpreter: Interpreter = Interpreter(scriptPath.parent)
        interpreter.interpret(statements)

    except FalxException as e:
        diagnostic = createDiagnostic(e)

        printer = DiagnosticPrinter()
        printer.print(diagnostic, source)
    except Exception: # baseline for actual language bug exceptions (all in language ones are from FalxRuntimeException/FalxException)
        traceback.print_exc()


if __name__ == "__main__":
    main()