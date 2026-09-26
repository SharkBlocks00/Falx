from __future__ import annotations

from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.ExpectedValueException import ExpectedValueException
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class ClampMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 3

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        try:
            return _clamp(arguments[0], arguments[1], arguments[2])
        except TypeError:
            raise ExpectedValueException(f"Cannot clamp '{arguments[0]}' to {arguments[1]}")

    def __eq__(self, other: object) -> bool:
        return isinstance(other, ClampMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)

def _clamp(n: FalxNumber, min: FalxNumber, max: FalxNumber) -> FalxNumber:
    if n.value < min.value: return min
    elif n.value > max.value: return max
    else: return n