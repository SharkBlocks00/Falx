from __future__ import annotations

from typing import TYPE_CHECKING

from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxThrowable import FalxThrowable
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter


class ExceptionFunction(NativeFunction):
    """Creates the base Falx exception value."""

    def isStrict(self) -> bool:
        return True

    def arity(self) -> int:
        return 1

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        return FalxThrowable(arguments[0].asString())

    def __eq__(self, other: object) -> bool:
        return isinstance(other, ExceptionFunction)

    def __hash__(self) -> int:
        return hash(ExceptionFunction)

    def __str__(self) -> str:
        return "<Exception constructor>"
