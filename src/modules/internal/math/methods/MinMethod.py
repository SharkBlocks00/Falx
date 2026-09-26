from __future__ import annotations

from typing import TYPE_CHECKING

from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class MinMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 2

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        return FalxNumber(min(arguments[0].asNumber(), arguments[1].asNumber()))

    def __eq__(self, other: object) -> bool:
        return isinstance(other, MinMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)