from __future__ import annotations

from typing import TYPE_CHECKING

from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class FracMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 1

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        number = arguments[0].asNumber()

        if number == 0 or number.is_integer():
            return FalxNumber(0)

        return FalxNumber(number - int(number))

    def __eq__(self, other: object) -> bool:
        return isinstance(other, FracMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)