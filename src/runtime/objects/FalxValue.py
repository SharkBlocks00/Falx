from __future__ import annotations

from abc import ABC, abstractmethod
from typing import Callable


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

        raise RuntimeError(f"{self.getTypeName()} has no property '{name}'") from None


    def set(self, name: str, value: FalxValue) -> None:
        """Set a property on this value.

        Subclasses that support settable properties must override this method
        """
        raise RuntimeError(f"{self.getTypeName()} has no settable property '{name}'") from None

    def index(self, index: FalxValue) -> FalxValue:
        """Return the value at the given index

        Subclasses that support indexing must override this
        """
        raise RuntimeError(f"{self.getTypeName()} is not indexable") from None

    def indexAssign(self, key: FalxValue, value: FalxValue) -> None:
        """Assign the given value to the given key

        Subclasses that support indexing must override this
        """
        raise RuntimeError(f"{self.getTypeName()} is not indexable") from None

    @abstractmethod
    def __eq__(self, other: FalxValue) -> bool:
        pass

    @abstractmethod
    def __hash__(self) -> int:
        pass

    def asNumber(self) -> int | float:
        """Return this value as a Python number in its literal value (int/float)"""
        raise RuntimeError(f"{self.getTypeName()} cannot be converted to a number") from None

    def add(self, other: FalxValue) -> FalxValue:
        """Add another FalxValue to this one"""
        raise RuntimeError(f"{self.getTypeName()} cannot be added to '{other.asString()}'")

    def minus(self, other: FalxValue) -> FalxValue:
        """Subtract another FalxValue from this one"""
        raise RuntimeError(f"{self.getTypeName()} cannot be subtracted from '{other.asString()}'")

    def multiply(self, other: FalxValue) -> FalxValue:
        """Multiply this FalxValue by another FalxValue"""
        raise RuntimeError(f"{self.getTypeName()} cannot be multiplied by '{other.asString()}'")

    def divide(self, other: FalxValue) -> FalxValue:
        """Divide this FalxValue by another FalxValue"""
        raise RuntimeError(f"{self.getTypeName()} cannot be divided by '{other.asString()}'")

    def mod(self, other: FalxValue) -> FalxValue:
        """Return the remainder of dividing this value by another FalxValue"""
        raise RuntimeError(f"Cannot mod {self.getTypeName()} with '{other.asString()}'")

    def compareTo(self, other: FalxValue) -> int:
        """Compare this value with another FalxValue"""
        raise RuntimeError(f"{self.getTypeName()} cannot be compared to '{other.asString()}'")

    def equalsValue(self, other: FalxValue) -> bool:
        """Determine whether this value is equal to another value at the lowest level"""
        raise RuntimeError(f"{self.getTypeName()} cannot be compared to '{other.asString()}'")

    def getTypeName(self) -> str:
        """Return the type name of this FalxValue"""
        return "object"

    def asInt(self) -> int:
        raise RuntimeError(f"Cannot convert '{self.__class__.__name__}' to an int")

    def asFloat(self) -> float:
        raise RuntimeError(f"Cannot convert '{self.__class__.__name__}' to a float")

    def asString(self) -> str:
        return self.__str__()

    def asBool(self) -> bool:
        raise RuntimeError(f"Cannot convert '{self.__class__.__name__}' to a bool")