from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.typing.UnexpectedTypeException import UnexpectedTypeException
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class InsertMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 2

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        from src.runtime.objects.FalxArray import FalxArray
        if not isinstance(self.this, FalxArray):
            raise UnexpectedTypeException(FalxArray.__class__.__name__, self.this.asString())

        array: FalxArray = self.this
        array.insert(arguments[0].asInt(), arguments[1])
        return self.this

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, InsertMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)