import sys

from src.packages.NativeFunction import NativeFunction
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxValue import FalxValue

class ReplExitFunction(NativeFunction):
    def __hash__(self) -> int:
        return hash(self.__class__)
    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, ReplExitFunction)
    def isStrict(self) -> bool:
        return False
    def arity(self) -> int:
        return 0
    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        sys.exit(0)

    def __str__(self):
        return "<repl builtin exit function>"