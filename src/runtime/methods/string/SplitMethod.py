from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.typing.UnexpectedTypeException import UnexpectedTypeException

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxArray import FalxArray
from src.runtime.objects.FalxValue import FalxValue


class SplitMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 1

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        from src.runtime.objects.FalxString import FalxString
        if not isinstance(self.this, FalxString):
            raise UnexpectedTypeException(FalxString.__class__.__name__, self.this.asString())

        string: FalxString = self.this
        arr: list[str] = string.asString().split(arguments[0].asString())
        output: list[FalxValue] = [FalxString(part) for part in arr]
        return FalxArray(output)

    def __eq__(self, other: object) -> bool:
        return isinstance(other, SplitMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)