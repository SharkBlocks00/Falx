from __future__ import annotations

from src.runtime.objects.FalxValue import FalxValue


class FalxNull(FalxValue):
    """Falx's class for representing null"""

    def __init__(self):
        super().__init__()

    def __eq__(self, other) -> bool:
        return isinstance(other, FalxNull)

    def __hash__(self) -> int:
        return hash(FalxNull)

    def getTypeName(self) -> str:
        return "null"

    def INSTANCE(self) -> FalxNull:
        return self

    def __str__(self) -> str:
        return "null"

    def __repr__(self) -> str:
        return "null"

    def asString(self) -> str:
        return "null"

    def equalsValue(self, other: FalxValue) -> bool:
        return isinstance(other, FalxNull)