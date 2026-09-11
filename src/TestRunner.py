import io
import sys
import time
from contextlib import redirect_stdout, redirect_stderr
from enum import Enum, auto
from pathlib import Path

from src.ast.Statement import Statement
from src.lexer.Lexer import Lexer
from src.parser.Parser import Parser
from src.runtime.Interpreter import Interpreter
from src.tokens.Token import Token


class ExpectedResult(Enum):
    SUCCESS = auto(),
    FAILURE = auto(),

class TestRunner:
    def __init__(self):
        self.RESET: str = "\u001B[0m"
        self.BOLD: str = "\u001B[1m"
        self.GREEN: str = "\u001B[32m"
        self.RED: str = "\u001B[31m"
        self.YELLOW: str = "\u001B[33m"
        self.CYAN: str = "\u001B[36m"

    def runAll(self) -> None:
        directory: Path = Path(__file__).parent.parent / "tests"

        files: list[Path] = sorted([item for item in directory.rglob("*") if item.is_file() and item.__str__().endswith(".flx")])

        passed: int = 0
        failed: int = 0

        suiteStart: float = time.perf_counter_ns()

        for path in files:
            relative: Path = path.relative_to(directory)
            category: str = relative.parts[0]

            match category:
                case "success" | "programs":
                    expected = ExpectedResult.SUCCESS
                case "failure":
                    expected = ExpectedResult.FAILURE
                case _:
                    raise ValueError(f"Unknown test category '{category}'")

            threw: bool = False
            exception: Exception | None = None

            # string buffers to catch output
            sink = io.StringIO()

            start: float = time.perf_counter_ns()

            try:
                with redirect_stdout(sink), redirect_stderr(sink):
                    source: str = path.read_text(encoding="utf-8")

                    lexer: Lexer = Lexer(source)
                    tokens: list[Token] = lexer.lex()

                    parser: Parser = Parser(tokens)
                    statements: list[Statement] = parser.parse()

                    interpreter: Interpreter = Interpreter()
                    interpreter.interpret(statements)
            except Exception as e:
                threw = True
                exception = e

            # time conversion to ms from ns
            elapsed: int = int((time.perf_counter_ns() - start) / 1_000_000)

            test_passed: bool = (
                expected == ExpectedResult.SUCCESS and not threw) or (
                expected == ExpectedResult.FAILURE and threw)

            relStr: str =str(relative)

            if test_passed:
                passed += 1
                print(f"{self.GREEN}[PASS]{self.RESET} {relStr:-<45} {self.CYAN}({elapsed} ms){self.RESET}")
            else:
                failed += 1
                print(f"{self.RED}[FAIL]{self.RESET} {relStr:-<45} {self.CYAN}({elapsed} ms){self.RESET}")

                if expected == ExpectedResult.SUCCESS:
                    print(f"       {exception}")
                else:
                    print("       Expected an exception but none was thrown")

        suiteTime: int = int((time.perf_counter_ns() - suiteStart) / 1_000_000)

        print()
        print(f"{self.BOLD}{'=' * 40}{self.RESET}")

        print(f"Tests      : {len(files)}")
        print(f"Passed     : {self.GREEN}{passed}{self.RESET}")
        print(f"Failed     : {self.GREEN if failed == 0 else self.RED}{failed}{self.RESET}")
        print(f"Total Time : {suiteTime} ms")

        print(f"{self.BOLD}{'=' * 40}{self.RESET}")

        if failed == 0:
            print()
            print(f"{self.GREEN}{self.BOLD}ALL TESTS PASSED{self.RESET}")
        else:
            print()
            plural: str = "" if failed == 1 else "S"
            print(
                f"{self.RED}{self.BOLD} {failed} TEST{plural} FAILED{self.RESET}")

        if failed > 0:
            sys.exit(1)