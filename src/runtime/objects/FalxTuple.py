from typing import Iterable

from src.diagnostics.exceptions.runtime.indexing.ArrayIndexInvalidException import ArrayIndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.InvalidIndexTypeException import InvalidIndexTypeException
from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.diagnostics.exceptions.runtime.typing.ImmutableValueException import ImmutableValueException
from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue


class FalxTuple(FalxValue, FalxIterable):
    def __init__(self, values: Iterable[FalxValue]):
        super().__init__()
        self.values: tuple[FalxValue, ...] = tuple(values)
        self._properties["size"] = lambda: FalxNumber(len(self.values))

    def iterate(self) -> Iterable[FalxValue]:
        return self.values

    def index(self, index: FalxValue) -> FalxValue:
        try:
            ArrayIndexInvalidException.check(index.asInt(), len(self.values))
        except CannotConvertToTypeException as e:
            raise InvalidIndexTypeException(e.message, e.location) from e
        return self.values[index.asInt()]

    def indexAssign(self, key: FalxValue, value: FalxValue) -> None:
        raise ImmutableValueException(self.getTypeName())

    def set(self, name: str, value: FalxValue) -> None:
        raise ImmutableValueException(self.getTypeName())

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
        return self.__repr__()

    def __repr__(self) -> str:
        if len(self.values) == 0:
            return "(,,)"
        if len(self.values) == 1:
            return f"({self.values[0]!r},)"
        return "(" + ", ".join(repr(val) for val in self.values) + ")"