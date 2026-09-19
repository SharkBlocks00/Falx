import traceback
from pathlib import Path
import sys

from src.ArgumentHandler import ArgumentHandler
from src.ast.Statement import Statement
from src.diagnostics.DiagnosticFactory import createDiagnostic
from src.diagnostics.DiagnosticPrinter import DiagnosticPrinter
from src.diagnostics.exceptions.cli.FalxCLIException import FalxCLIException
from src.diagnostics.exceptions.FalxException import FalxException
from src.lexer.Lexer import Lexer
from src.parser.Parser import Parser
from src.runtime.Interpreter import Interpreter
from src.runtime.methods.repl.ReplExitFunction import ReplExitFunction
from src.tokens.Token import Token


def repl() -> None:
    interpreter = Interpreter(Path.cwd())
    interpreter.globals.define("exit", ReplExitFunction(), False)
    
    print("Falx REPL")
    print("Type 'exit()' to exit")
    
    while True:
        try:
            line = input(">> ")
        except (EOFError, KeyboardInterrupt):
            print()
            break
            
        if not line.strip():
            continue
            
        try:
            lexer: Lexer = Lexer(line)
            tokens: list[Token] = lexer.lex()

            parser: Parser = Parser(tokens)
            statements: list[Statement] = parser.parse()

            interpreter.interpret(statements)
        except FalxException as e:
            diagnostic = createDiagnostic(e)
            printer = DiagnosticPrinter()
            printer.print(diagnostic, line)
        except Exception:
            traceback.print_exc()

def main():
    argumentHandler: ArgumentHandler = ArgumentHandler(sys.argv)

    try:
        scriptPath: Path | None = argumentHandler.handleArguments()
    except FalxCLIException as e:
        diagnostic = createDiagnostic(e)

        printer = DiagnosticPrinter()
        printer.print(diagnostic, "")
        return 1

    if scriptPath is None:
        if len(sys.argv) == 1:
            repl()
        return 0

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
        return 1
    except Exception: # baseline for actual language bug exceptions (all in language ones are from FalxRuntimeException/FalxException)
        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())