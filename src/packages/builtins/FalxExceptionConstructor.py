from __future__ import annotations

from typing import TYPE_CHECKING

from src.diagnostics.exceptions.FalxException import FalxException
from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxExceptionValue import FalxExceptionValue
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter


class FalxExceptionConstructor(NativeFunction):
    """Constructs a specific Falx diagnostic exception with a user message."""

    def __init__(self, exceptionType: type[FalxException]):
        super().__init__()
        self.exceptionType = exceptionType

    def isStrict(self) -> bool:
        return False

    def arity(self) -> int:
        return 1

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        exception = self.exceptionType.__new__(self.exceptionType)
        try:
            arg = arguments[0].asString()
        except IndexError:
            arg = ""
        FalxException.__init__(exception, arg)
        return FalxExceptionValue(exception)

    def __eq__(self, other: object) -> bool:
        return isinstance(other, FalxExceptionConstructor) and self.exceptionType is other.exceptionType

    def __hash__(self) -> int:
        return hash((FalxExceptionConstructor, self.exceptionType))

    def __str__(self) -> str:
        return f"<{self.exceptionType.__name__} constructor>"
