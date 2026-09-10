from __future__ import annotations
from typing import TYPE_CHECKING

from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class ReverseMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 0

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        from src.runtime.objects.FalxArray import FalxArray
        if not isinstance(self.this, FalxArray):
            raise RuntimeError(f"Expected FalxArray, got {type(self.this)}")

        array: FalxArray = self.this
        array.asList().reverse()
        return array

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, ReverseMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)
