from typing import Iterable

from src.diagnostics.exceptions.FalxException import FalxException
from src.diagnostics.exceptions.runtime.indexing.MapIndexInvalidException import MapIndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.CannotHashObjectException import CannotHashObjectException
from src.runtime.methods.maps.ClearMethod import ClearMethod
from src.runtime.methods.maps.ContainsMethod import ContainsMethod
from src.runtime.methods.maps.CopyMethod import CopyMethod
from src.runtime.methods.maps.EmptyMethod import EmptyMethod
from src.runtime.methods.maps.EntiresMethod import EntriesMethod
from src.runtime.methods.maps.KeysMethod import KeysMethod
from src.runtime.methods.maps.RemoveMethod import RemoveMethod
from src.runtime.methods.maps.ValuesMethod import ValuesMethod
from src.runtime.objects.FalxArray import FalxArray
from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxValue import FalxValue


class FalxMap(FalxValue, FalxIterable):
    def __init__(self, values: dict[FalxValue, FalxValue]):
        super().__init__()
        self.values: dict[FalxValue, FalxValue] = values
        self._properties["size"] = lambda: FalxNumber(len(self.values))
        self._methods["keys"] = KeysMethod(self)
        self._methods["isEmpty"] = EmptyMethod(self)
        self._methods["contains"] = ContainsMethod(self)
        self._methods["copy"] = CopyMethod(self)
        self._methods["remove"] = RemoveMethod(self)
        self._methods["clear"] = ClearMethod(self)
        self._methods["values"] = ValuesMethod(self)
        self._methods["entries"] = EntriesMethod(self)

    def get(self, name: str) -> FalxValue:
        if FalxString(name) not in self.values:
            try:
                # use the base FalxValue class's get first
                return super().get(name)
            except FalxException:
                return self.values.get(FalxString(name), FalxNull())


        return self.values.get(FalxString(name), FalxNull())

    def index(self, index: FalxValue) -> FalxValue:
        return self.values.get(index, FalxNull())

    def indexAssign(self, key: FalxValue, value: FalxValue) -> None:
        try:
            self.values[key] = value
        except TypeError:
            raise CannotHashObjectException(key.asString(), key.getTypeName())

    def getTypeName(self) -> str:
        return "map"

    def set(self, name: str, value: FalxValue) -> None:
        self.values[FalxString(name)] = value

    def iterate(self) -> Iterable[FalxValue]:
        return self.values.values()

    def equalsValue(self, other: FalxValue) -> bool:
        if not isinstance(other, FalxMap):
            return False

        return self.values == other.values

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, FalxMap) and self.values == other.values


    def __str__(self) -> str:
        return self.values.__str__()

    def __repr__(self) -> str:
        return self.values.__repr__()

    def getKeys(self) -> FalxArray:
        return FalxArray([i for i in self.values.keys()])

    def getValues(self) -> FalxArray:
        return FalxArray([i for i in self.values.values()])

    def isEmpty(self) -> bool:
        return len(self.values) == 0

    def getMap(self) -> dict[FalxValue, FalxValue]:
        return self.values.copy()

    def remove(self, key: FalxValue) -> None:
        try:
            self.values.pop(key)
        except KeyError:
            raise MapIndexInvalidException(key.asString(), len(self.values))

    def clear(self) -> None:
        self.values = {}

