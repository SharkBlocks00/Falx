import sys
from pathlib import Path

from src.TestRunner import TestRunner


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
                        raise Exception(f"Missing filename after '{self.arguments[i]}'")

                    path: Path = Path(self.arguments[i + 1])

                    if path.suffix != ".flx":
                        raise Exception(f"Invalid filetype after '{self.arguments[i]}'")
                    return path

                case "--test" | "-t":
                    testRunner.runAll()
        return None