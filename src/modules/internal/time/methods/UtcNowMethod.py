from __future__ import annotations

from datetime import datetime, timezone
from typing import TYPE_CHECKING

from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxMap import FalxMap
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter


class UtcNowMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 0

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        now = datetime.now(timezone.utc)

        return FalxMap({
            FalxString("year"): FalxNumber(now.year),
            FalxString("month"): FalxNumber(now.month),
            FalxString("day"): FalxNumber(now.day),
            FalxString("hour"): FalxNumber(now.hour),
            FalxString("minute"): FalxNumber(now.minute),
            FalxString("second"): FalxNumber(now.second),
            FalxString("microsecond"): FalxNumber(now.microsecond),
        })

    def __eq__(self, other: object) -> bool:
        return isinstance(other, UtcNowMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)