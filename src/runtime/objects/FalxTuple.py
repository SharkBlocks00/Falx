from typing import Iterable

from src.diagnostics.exceptions.runtime.indexing.ArrayIndexInvalidException import ArrayIndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.InvalidIndexTypeException import InvalidIndexTypeException
from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxValue import FalxValue


class FalxTuple(FalxValue, FalxIterable):
    def __init__(self, values: list[FalxValue]):
        super().__init__()
        self.values: list[FalxValue] = values

    def iterate(self) -> Iterable[FalxValue]:
        return self.values

    def index(self, index: FalxValue) -> FalxValue:
        try:
            ArrayIndexInvalidException.check(index.asInt(), len(self.values))
        except CannotConvertToTypeException as e:
            raise InvalidIndexTypeException(e.message, e.location) from e
        return self.values[index.asInt()]

    def equalsValue(self, other: FalxValue) -> bool:
        if not isinstance(other, FalxTuple):
            return False

        return self.values == other.values

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, FalxTuple) and self.values == other.values

    def __hash__(self) -> int:
        return hash(self.values)

    def getTypeName(self) -> str:
        return "tuple"

    def __str__(self) -> str:
        return str(self.values)

    def __repr__(self) -> str:
        return ", ".join(repr(val) for val in self.values)

