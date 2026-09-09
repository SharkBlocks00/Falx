from typing import Iterable

from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.objects.FalxNumber import FalxNumber


class FalxArray(FalxValue, FalxIterable):
    def __init__(self, values: list[FalxValue]):
        super().__init__()
        self.values: list[FalxValue] = values
        self._properties["size"] = lambda: FalxNumber(len(self.values))

    def iterate(self) -> Iterable[FalxValue]:
        return self.values

    def index(self, index: FalxValue) -> FalxValue:
        return self.values[index.asInt()]

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, FalxArray) and self.values == other.values

    def __hash__(self) -> int:
        return hash(self.values)

    def getTypeName(self) -> str:
        return "array"

    def __str__(self) -> str:
        return str(self.values)

    def push(self, value: FalxValue) -> None:
        self.values.append(value)

    def pop(self) -> FalxValue:
        return self.values.pop()

    def insert(self, index: int, value: FalxValue) -> None:
        self.values.insert(index, value)

    def remove(self, value: FalxValue) -> None:
        self.values.remove(value)

    def clear(self) -> None:
        self.values = []

    def contains(self, value: FalxValue) -> bool:
        return value in self.values

    def size(self) -> int:
        return len(self.values)

    def asList(self) -> list[FalxValue]:
        return self.values

    def isEmpty(self) -> bool:
        return len(self.values) == 0