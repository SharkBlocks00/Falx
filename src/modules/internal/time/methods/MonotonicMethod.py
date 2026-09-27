import time

from src.runtime.Interpreter import Interpreter
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

class MonotonicMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 0

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        return FalxNumber(time.monotonic())

    def __eq__(self, other: object) -> bool:
        return isinstance(other, MonotonicMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)
