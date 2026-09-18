import sys
from pathlib import Path

from src.TestRunner import TestRunner
from src.diagnostics.exceptions.cli.MissingFilenameException import MissingFilenameException
from src.diagnostics.exceptions.cli.InvalidFileTypeException import InvalidFileTypeException


testRunner: TestRunner = TestRunner()

class ArgumentHandler:
    def __init__(self, arguments: list[str]):
        self.arguments: list[str] = arguments

    def handleArguments(self) -> Path | None:
        for i, argument in enumerate(self.arguments):
            match argument:
                case "--version" | "-v":
                    print("Falx v0.1.0")
                    sys.exit(0)
                case "--file" | "-f" | "run":
                    if i + 1 >= len(self.arguments):
                        raise MissingFilenameException(self.arguments[i])

                    path: Path = Path(self.arguments[i + 1])

                    if path.suffix != ".flx":
                        raise InvalidFileTypeException(self.arguments[i])
                    return path

                case "--test" | "-t":
                    testRunner.runAll()
        return None