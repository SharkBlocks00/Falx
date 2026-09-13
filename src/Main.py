import traceback
from pathlib import Path
import sys

from src.ArgumentHandler import ArgumentHandler
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.exceptions.exceptions.FalxException import FalxException
from src.lexer.Lexer import Lexer
from src.parser.Parser import Parser
from src.runtime.Interpreter import Interpreter
from src.tokens.Token import Token

def printErrorContext(source: str, error: FalxException) -> None:
    location: SourceLocation = error.location

    if location is None:
        print(f"{type(error).__name__}: {error.message}", file=sys.stderr)
        return

    lines = source.splitlines()

    print(f"{location}: {type(error).__name__}: {error.message}", file=sys.stderr)

    if location.line > len(lines): return

    errorLine = lines[location.line - 1]
    lineNumberWidth = len(str(len(lines)))

    print(f"  {location.line:>{lineNumberWidth}} | {errorLine}", file=sys.stderr)

    print(
        f"  {' ' * lineNumberWidth} |  {' ' * (location.column - 1)}^", file=sys.stderr)

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
        printErrorContext(source, e)
    except Exception: # baseline for actual language bug exceptions (all in language ones are from FalxRuntimeException)
        traceback.print_exc()


if __name__ == "__main__":
    main()