from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue


class AssertFunction(NativeFunction):
    """assert(condition) or assert(condition, message).

    Raises a RuntimeError if `condition` is false
    """

    def __hash__(self) -> int:
        return hash(self.__class__)

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, AssertFunction)

    def isStrict(self) -> bool:
        return False

    def arity(self) -> int:
        return 1

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        if len(arguments) == 0:
            raise RuntimeError("assert() requires at least a condition argument")

        condition: FalxValue = arguments[0]

        if not condition.asBool():
            if len(arguments) > 1:
                message: str = arguments[1].asString()
                raise RuntimeError(f"Assertion failed: {message}")
            raise RuntimeError("Assertion failed")

        return FalxNull()

    def __str__(self):
        return "<assert function>"