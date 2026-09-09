from __future__ import annotations

from src.runtime.objects.FalxValue import FalxValue


class FalxNull(FalxValue):
    """Falx's class for representing null,
    used internally in workings where None is not accepted,
    and externally as placeholders for null values in Falx programs
    """
    def __init__(self):
        super().__init__()

    def __eq__(self, other) -> bool:
        return isinstance(other, FalxNull)

    def __hash__(self) -> int:
        return hash(self)

    def getTypeName(self) -> str:
        return "null"

    def INSTANCE(self) -> FalxNull:
        return self

    def __str__(self) -> str:
        return "null"

    def asString(self) -> str:
        return "null"