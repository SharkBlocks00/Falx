from __future__ import annotations
from typing import TYPE_CHECKING

from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxArray import FalxArray
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class EntriesMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 0

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        from src.runtime.objects.FalxMap import FalxMap
        if not isinstance(self.this, FalxMap):
            raise RuntimeError(f"Expected FalxMap, got {type(self.this)}")

        map: FalxMap = self.this
        entries: list[FalxValue] = []

        for k, v in enumerate(map.getMap()):
            pair: list[FalxValue] = [v, k]

            entries.append(FalxArray(pair))

        return FalxArray(entries)

    def __eq__(self, other: EntriesMethod) -> bool:
        return isinstance(other, EntriesMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)