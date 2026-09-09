from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxValue import FalxValue


class RequestFunction(NativeFunction):
    def arity(self) -> int:
        return 1

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        if len(arguments) == 0:
            return FalxString(input())
        print(arguments[0])
        return FalxString(input())

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, RequestFunction)

    def __hash__(self) -> int:
        return hash(self.__class__)