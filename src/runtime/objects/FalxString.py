from typing import Iterable

from src.diagnostics.exceptions.runtime.indexing.StringIndexInvalidException import StringIndexInvalidException
from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.runtime.methods.string.ContainMethod import ContainMethod
from src.runtime.methods.string.EndsWithMethod import EndsWithMethod
from src.runtime.methods.string.IntMethod import IntMethod
from src.runtime.methods.string.LowerMethod import LowerMethod
from src.runtime.methods.string.RealMethod import RealMethod
from src.runtime.methods.string.ReplaceMethod import ReplaceMethod
from src.runtime.methods.string.SplitMethod import SplitMethod
from src.runtime.methods.string.StartsWithMethod import StartsWithMethod
from src.runtime.methods.string.TrimMethod import TrimMethod
from src.runtime.methods.string.UpperMethod import UpperMethod
from src.runtime.objects.FalxBoolean import FalxBoolean
from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue


class FalxString(FalxValue, FalxIterable):
    def __init__(self, value: str):
        super().__init__()
        self.value: str = value
        self._properties["length"] = lambda: FalxNumber(len(value))
        self._methods["toUpper"] = UpperMethod(self)
        self._methods["toLower"] = LowerMethod(self)
        self._methods["trim"] = TrimMethod(self)
        self._methods["toInt"] = IntMethod(self)
        self._methods["toReal"] = RealMethod(self)
        self._methods["trim"] = TrimMethod(self)
        self._methods["contains"] = ContainMethod(self)
        self._methods["endsWith"] = EndsWithMethod(self)
        self._methods["startsWith"] = StartsWithMethod(self)
        self._methods["replace"] = ReplaceMethod(self)
        self._methods["split"] = SplitMethod(self)

    def asString(self) -> str:
        return self.value

    def asInt(self) -> int:
        try:
            return int(self.value)
        except ValueError:
            raise CannotConvertToTypeException(self.value, "int")

    def asFloat(self) -> float:
        try:
            return float(self.value)
        except ValueError:
            raise CannotConvertToTypeException(self.value, "float")

    def asBool(self) -> bool:
        return self.value.lower() == "true"

    def equalsValue(self, other: FalxValue) -> bool:
        if not isinstance(other, FalxString):
            return False

        return self.value == other.value

    def __str__(self):
        return self.value

    def __repr__(self):
        return self.value

    def __eq__(self, other: object) -> bool:
        return isinstance(other, FalxString) and self.value == other.value

    def __hash__(self) -> int:
        return hash(self.value)

    def index(self, index: FalxValue) -> FalxValue:
        i: int = index.asInt()
        StringIndexInvalidException.check(i, len(self.value))
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

