from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue


class RealMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 0

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        from src.runtime.objects.FalxString import FalxString
        if not isinstance(self.this, FalxString):
            raise TypeError(f"Expected FalxString, got {type(self).__name__}")

        string: FalxString = self.this
        return FalxNumber(string.asFloat())

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, RealMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)