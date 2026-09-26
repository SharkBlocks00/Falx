from __future__ import annotations

from pathlib import Path
from typing import TYPE_CHECKING

from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxFile import FalxFile
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class OpenMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 2

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        path = Path(arguments[0].asString())
        mode = arguments[1].asString()

        if not path.is_absolute() and interpreter.currentFile is not None:
            path = interpreter.currentFile.parent / path

        path = path.resolve()

        file = open(path, mode)

        return FalxFile(file)



    def __eq__(self, other: object) -> bool:
        return isinstance(other, OpenMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)