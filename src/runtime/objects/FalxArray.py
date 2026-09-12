from typing import Iterable

from src.runtime.methods.array.ClearMethod import ClearMethod
from src.runtime.methods.array.ContainsMethod import ContainsMethod
from src.runtime.methods.array.CopyMethod import CopyMethod
from src.runtime.methods.array.EmptyMethod import EmptyMethod
from src.runtime.methods.array.InsertMethod import InsertMethod
from src.runtime.methods.array.PopMethod import PopMethod
from src.runtime.methods.array.PushMethod import PushMethod
from src.runtime.methods.array.RemoveMethod import RemoveMethod
from src.runtime.methods.array.ReverseMethod import ReverseMethod
from src.runtime.methods.array.SortMethod import SortMethod
from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.objects.FalxNumber import FalxNumber


class FalxArray(FalxValue, FalxIterable):
    def __init__(self, values: list[FalxValue]):
        super().__init__()
        self.values: list[FalxValue] = values
        self._properties["size"] = lambda: FalxNumber(len(self.values))
        self._methods["clear"] = ClearMethod(self)
        self._methods["contains"] = ContainsMethod(self)
        self._methods["copy"] = CopyMethod(self)
        self._methods["isEmpty"] = EmptyMethod(self)
        self._methods["insert"] = InsertMethod(self)
        self._methods["pop"] = PopMethod(self)
        self._methods["push"] = PushMethod(self)
        self._methods["remove"] = RemoveMethod(self)
        self._methods["reverse"] = ReverseMethod(self)
        self._methods["sort"] = SortMethod(self)

    def iterate(self) -> Iterable[FalxValue]:
        return self.values

    def index(self, index: FalxValue) -> FalxValue:
        return self.values[index.asInt()]

    def indexAssign(self, key: FalxValue, value: FalxValue) -> None:
        self.values[key.asInt()] = value

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, FalxArray) and self.values == other.values

    def __hash__(self) -> int:
        return hash(self.values)

    def getTypeName(self) -> str:
        return "array"

    def __str__(self) -> str:
        return str(self.values)

    def __repr__(self) -> str:
        return ", ".join(repr(val) for val in self.values)

    def push(self, value: FalxValue) -> None:
        self.values.append(value)

    def pop(self) -> FalxValue:
        return self.values.pop()

    def insert(self, index: int, value: FalxValue) -> None:
        self.values.insert(index, value)

    def remove(self, index: int) -> FalxValue:
        return self.values.pop(index)

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