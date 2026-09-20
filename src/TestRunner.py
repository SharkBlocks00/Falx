import io
import sys
import time
from contextlib import redirect_stdout, redirect_stderr
from enum import Enum, auto
from multiprocessing import Process, Queue
from pathlib import Path
from queue import Empty

from src.ast.Statement import Statement
from src.diagnostics.DiagnosticCode import DiagnosticCode
from src.diagnostics.DiagnosticRegistry import DIAGNOSTIC_REGISTRY
from src.diagnostics.exceptions.FalxException import FalxException
from src.lexer.Lexer import Lexer
from src.parser.Parser import Parser
from src.runtime.Interpreter import Interpreter
from src.tokens.Token import Token

class ExpectedResult(Enum):
    SUCCESS = auto(),
    FAILURE = auto(),

DIAGNOSTIC_CODE_REGISTRY = {
    definition.code: exception
    for exception, definition in DIAGNOSTIC_REGISTRY.items()
}

def _executeTest(path: Path) -> dict:
    sink = io.StringIO()

    try:
        with redirect_stdout(sink), redirect_stderr(sink):
            source: str = path.read_text(encoding="utf-8")

            lexer: Lexer = Lexer(source, filename=path.name)
            tokens: list[Token] = lexer.lex()

            parser: Parser = Parser(tokens)
            statements: list[Statement] = parser.parse()

            interpreter: Interpreter = Interpreter(path.parent)
            interpreter.interpret(statements)

        return {
            "success": True,
            "exception": None,
            "output": sink.getvalue(),
        }

    except Exception as e:
        return {
            "success": False,
            "exception": e.__class__.__name__,
            "message": str(e),
            "output": sink.getvalue(),
        }

def _testWorker(testQueue: Queue, resultQueue: Queue) -> None:
    while True:
        path: Path = testQueue.get()
        if path is None:
            break

        resultQueue.put(_executeTest(path))


class TestRunner:
    def __init__(self):
        self.RESET: str = "\u001B[0m"
        self.BOLD: str = "\u001B[1m"
        self.GREEN: str = "\u001B[32m"
        self.RED: str = "\u001B[31m"
        self.YELLOW: str = "\u001B[33m"
        self.CYAN: str = "\u001B[36m"


        self.TEST_TIMEOUT: float = 1.0 # one second

    def runAll(self) -> None:
        directory: Path = Path(__file__).parent.parent / "tests"

        files: list[Path] = sorted([item for item in directory.rglob("*") if item.is_file() and item.__str__().endswith(".flx")])

        passed: int = 0
        failed: int = 0

        suiteStart: float = time.perf_counter_ns()

        testQueue: Queue = Queue()
        resultQueue: Queue = Queue()

        process = Process(
            target=_testWorker,
            args=(testQueue, resultQueue),
        )

        process.start()

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

            source: str = path.read_text(encoding="utf-8")
            expectedCode: str | None = self.getExpectedCode(source)

            start: float = time.perf_counter_ns()

            testQueue.put(path)

            try:
                testResult = resultQueue.get(timeout=self.TEST_TIMEOUT)

                elapsed: int = int(
                    (time.perf_counter_ns() - start) / 1_000_000
                )

                timed_out = False

                threw: bool = not testResult["success"]

                exceptionName: str | None = testResult["exception"]
                exceptionMessage: str | None = testResult.get("message")

                ex: DiagnosticCode | None = None
                expectedException: type[FalxException] | None = None

                if expectedCode is not None:
                    ex = DiagnosticCode(expectedCode)
                    expectedException = DIAGNOSTIC_CODE_REGISTRY[ex]

                expectedExceptionName = (expectedException.__name__ if expectedException is not None else None)

                test_passed = (expected == ExpectedResult.SUCCESS and not threw) or (
                    expected == ExpectedResult.FAILURE and threw and exceptionName == expectedExceptionName
                )

                exception = (exceptionMessage if exceptionMessage is not None else None)

            except Empty:
                elapsed: int = int(
                    (time.perf_counter_ns() - start) / 1_000_000
                )

                timed_out = True
                test_passed = False
                exception = None

                process.terminate()
                process.join()

                testQueue = Queue()
                resultQueue = Queue()

                process = Process(
                    target=_testWorker,
                    args=(testQueue, resultQueue),
                )

                process.start()

            relStr: str = str(relative)

            if test_passed:
                passed += 1
                print(f"{self.GREEN}[PASS]{self.RESET} {relStr:-<45} {self.CYAN}({elapsed} ms){self.RESET}")
            else:
                failed += 1
                print(f"{self.RED}[FAIL]{self.RESET} {relStr:-<45} {self.CYAN}({elapsed} ms){self.RESET}")

                if timed_out:
                    print(
                        f"       Test exceeded "
                        f"{self.TEST_TIMEOUT:g} second timeout"
                    )
                elif expected == ExpectedResult.SUCCESS:
                    print(f"       {exception}")
                else:
                    print("       Expected an exception but none was thrown")

        testQueue.put(None)
        process.join()

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


    def getExpectedCode(self, source: str) -> str | None:
        firstLine: str = source.splitlines()[0]

        if not firstLine.startswith("// Expects:"):
            return None

        return firstLine.split(":",1)[1].strip().split()[0]

if __name__ == "__main__":
    TestRunner().runAll()
