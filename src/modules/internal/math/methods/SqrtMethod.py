from __future__ import annotations

import math
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.ExpectedValueException import ExpectedValueException
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class SqrtMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 1

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        if arguments[0].asNumber() < 0:
            raise ExpectedValueException("Cannot square root a negative number.")
        return FalxNumber(math.sqrt(arguments[0].asNumber()))

    def __eq__(self, other: object) -> bool:
        return isinstance(other, SqrtMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)