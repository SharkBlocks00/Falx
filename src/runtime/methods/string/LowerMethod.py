from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxValue import FalxValue


class LowerMethod(BoundNativeFunction):
    def isStrict(self) -> bool:
        return True

    def __init__(self, this: FalxValue):
        super().__init__(this)

    def arity(self) -> int:
        return 0

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        from src.runtime.objects.FalxString import FalxString
        if not isinstance(self.this, FalxString):
            raise TypeError(f"Expected FalxString, got {type(self).__name__}")
        string: FalxString = self.this

        return FalxString(string.asString().lower())

    def __eq__(self, other: object) -> bool:
        return isinstance(other, LowerMethod) and super().__eq__(other)

    def __hash__(self) -> int:
        return super().__hash__()
