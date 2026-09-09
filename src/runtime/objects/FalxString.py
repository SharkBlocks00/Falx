from typing import Iterable

from src.runtime.objects.FalxBoolean import FalxBoolean
from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue


class FalxString(FalxValue, FalxIterable):
    def __init__(self, value: str):
        super().__init__()
        self.value: str = value
        self._properties["length"] = lambda: FalxNumber(len(value))

    def asString(self) -> str:
        return self.value

    def asInt(self) -> int:
        try:
            return int(self.value)
        except ValueError:
            raise RuntimeError(f"Cannot convert {self.value} to int")

    def equalsValue(self, other: FalxValue) -> bool:
        if isinstance(other, FalxString):
            return self.value == other.value
        else:
            return self.asFloat() == other.asFloat()

    def __str__(self):
        return self.value

    def __eq__(self, other: object) -> bool:
        return isinstance(other, FalxString) and self.value == other.value

    def __hash__(self) -> int:
        return hash(self.value)

    def index(self, index: FalxValue) -> FalxValue:
        i: int = index.asInt()
        return FalxString(self.value[i])

    def iterate(self) -> Iterable[FalxValue]:
        values: list[FalxValue] = []
        for c in self.value:
            values.append(FalxString(c))

        return values

    def add(self, other: FalxValue) -> FalxValue:
        if isinstance(other, FalxString):
            return FalxString(self.value + other.value)
        elif isinstance(other, FalxNumber):
            return FalxString(self.value + other.asString())
        elif isinstance(other, FalxBoolean):
            return FalxString(self.value + other.asString())
        return super().add(other)

