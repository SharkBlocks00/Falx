from __future__ import annotations

from abc import ABC, abstractmethod
from typing import Callable, TYPE_CHECKING

from src.diagnostics.exceptions.runtime.operations.CannotAddWithValueException import CannotAddWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotCompareToException import CannotCompareToException
from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.diagnostics.exceptions.runtime.operations.CannotDivideByValueException import CannotDivideByValueException
from src.diagnostics.exceptions.runtime.operations.CannotMinusFromValueException import CannotMinusFromValueException
from src.diagnostics.exceptions.runtime.operations.CannotModWithValueException import CannotModWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotMultiplyByValue import CannotMultiplyByValueException
from src.diagnostics.exceptions.runtime.properties.CannotAccessPropertyException import CannotAccessPropertyException
from src.diagnostics.exceptions.runtime.properties.CannotSetPropertyException import CannotSetPropertyException
from src.diagnostics.exceptions.runtime.properties.ObjectIsNotIndexableException import ObjectIsNotIndexableException

if TYPE_CHECKING:
    from src.packages.NativeFunction import NativeFunction


class FalxValue(ABC):
    """Base class for all runtime values in Falx"""
    def __init__(self):
        self._methods: dict[str, NativeFunction] = {}
        self._properties: dict[str, Callable[[], FalxValue]] = {}

    def get(self, name: str) -> FalxValue:
        """Return the property or method with the given name"""
        _property = self._properties.get(name)
        if _property is not None:
            return _property()

        function: NativeFunction = self._methods.get(name)
        if function is not None:
            return function

        raise CannotAccessPropertyException(f"{self.getTypeName()} has no property '{name}'") from None


    def set(self, name: str, value: FalxValue) -> None:
        """Set a property on this value.

        Subclasses that support settable properties must override this method
        """
        raise CannotSetPropertyException(self.getTypeName(), name) from None

    def index(self, index: FalxValue) -> FalxValue:
        """Return the value at the given index

        Subclasses that support indexing must override this
        """
        raise ObjectIsNotIndexableException(self.getTypeName()) from None

    def indexAssign(self, key: FalxValue, value: FalxValue) -> None:
        """Assign the given value to the given key

        Subclasses that support indexing must override this
        """
        raise ObjectIsNotIndexableException(self.getTypeName()) from None

    @abstractmethod
    def __eq__(self, other: FalxValue) -> bool:
        pass

    @abstractmethod
    def __hash__(self) -> int:
        pass


    def add(self, other: FalxValue) -> FalxValue:
        """Add another FalxValue to this one"""
        raise CannotAddWithValueException(self.getTypeName(), other.asString())

    def minus(self, other: FalxValue) -> FalxValue:
        """Subtract another FalxValue from this one"""
        raise CannotMinusFromValueException(self.getTypeName(), other.asString())

    def multiply(self, other: FalxValue) -> FalxValue:
        """Multiply this FalxValue by another FalxValue"""
        raise CannotMultiplyByValueException(other.asString(), self.getTypeName())

    def divide(self, other: FalxValue) -> FalxValue:
        """Divide this FalxValue by another FalxValue"""
        raise CannotDivideByValueException(other.asString(), self.getTypeName())

    def mod(self, other: FalxValue) -> FalxValue:
        """Return the remainder of dividing this value by another FalxValue"""
        raise CannotModWithValueException(self.getTypeName(), other.asString())

    def compareTo(self, other: FalxValue) -> int:
        """Compare this value with another FalxValue"""
        raise CannotCompareToException(self.getTypeName(), other.asString())

    def equalsValue(self, other: FalxValue) -> bool:
        """Determine whether this value is equal to another value at the lowest level"""
        raise CannotCompareToException(self.getTypeName(), other.asString())

    def getTypeName(self) -> str:
        """Return the type name of this FalxValue"""
        return "object"

    def asNumber(self) -> int | float:
        """Return this value as a Python number in its literal value (int/float)"""
        raise CannotConvertToTypeException(self.getTypeName(), "number")

    def asInt(self) -> int:
        raise CannotConvertToTypeException(self.getTypeName(), "int")

    def asFloat(self) -> float:
        raise CannotConvertToTypeException(self.getTypeName(), "float")

    def asString(self) -> str:
        return self.__str__()

    def asBool(self) -> bool:
        raise CannotConvertToTypeException(self.getTypeName(), "bool")