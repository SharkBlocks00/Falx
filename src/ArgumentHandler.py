import sys
from src.TestRunner import TestRunner


testRunner: TestRunner = TestRunner()

class ArgumentHandler:
    def __init__(self, arguments: list[str]):
        self.arguments: list[str] = arguments

    def handleArguments(self) -> object:
        for i, argument in enumerate(self.arguments):
            match self.arguments[i]:
                case "--version" | "-v":
                    print("Falx v0.1.0")
                    sys.exit(0)
                case "--file" | "-f" | "run":
                    if i + 1 >= len(self.arguments):
                        raise Exception(f"Missing filename after '{self.arguments[i]}'")
                    with open(self.arguments[i]) as file:
                      contents: str = file.read()
                      return contents
                case "--test" | "-t":
                    testRunner.runAll()
        return None