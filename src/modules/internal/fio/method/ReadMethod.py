from __future__ import annotations

from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.ExpectedValueException import ExpectedValueException
from src.diagnostics.exceptions.runtime.methods.InvalidArgumentCountException import InvalidArgumentCountException
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxFile import FalxFile
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class ReadMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 2

    def isStrict(self) -> bool:
        return False

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:

        if len(arguments) <= 0 or len(arguments) > 2:
            raise InvalidArgumentCountException(len(arguments), 2)

        file = arguments[0]

        if not isinstance(file, FalxFile):
            raise ExpectedValueException(f"Expected file, got '{file.getTypeName()}'")

        size: int | None
        try:
            size = arguments[1].asInt()
        except IndexError:
            size = None

        return FalxString(file.file.read(size))

    def __eq__(self, other: object) -> bool:
        return isinstance(other, ReadMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)