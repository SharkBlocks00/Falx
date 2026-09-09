from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxValue import FalxValue


class TypeOfFunction(NativeFunction):
    def arity(self) -> int:
        return 1

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        return FalxString(arguments[0].__class__.__name__) # returns the string value of the type

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, TypeOfFunction)

    def __hash__(self) -> int:
        return hash(self.__class__)