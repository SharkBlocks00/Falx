from __future__ import annotations

from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.ExpectedValueException import ExpectedValueException
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxFile import FalxFile
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class WriteMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 1

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        file = arguments[0]
        data = arguments[1].asString()

        if not isinstance(file, FalxFile):
            raise ExpectedValueException(
                f"Expected file, got '{file.getTypeName()}'"
            )

        written = file.file.write(data)

        return FalxNumber(written)

    def __eq__(self, other: object) -> bool:
        return isinstance(other, WriteMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)