from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue


class OutputFunction(NativeFunction):

    def __hash__(self) -> int:
        return hash(self.__class__)

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, OutputFunction)

    def arity(self) -> int:
        return 1

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        if len(arguments) == 0:
            # arguments are null
            print()
            return FalxNull()

        print(arguments[0]) # print just the first argument for now TODO: make it accept more
        return FalxNull()

    def __str__(self):
        return "<output function>"

