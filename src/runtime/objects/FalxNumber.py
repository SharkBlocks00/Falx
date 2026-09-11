from __future__ import annotations


from src.runtime.objects.FalxValue import FalxValue

class FalxNumber(FalxValue):
    def __init__(self, value: int | float):
        super().__init__()
        self.value = value

    def __str__(self):
        return str(self.value)

    def __repr__(self):
        return str(self.value)

    def asInt(self) -> int:
        return int(self.value)

    def asFloat(self) -> float:
        return float(self.value)

    def asNumber(self) -> int | float:
        return self.value

    def add(self, other: FalxValue) -> FalxValue:
        from src.runtime.objects.FalxString import FalxString
        if isinstance(other, FalxString):
            return FalxString(self.asString() + other.asString())

        if isinstance(other, FalxNumber):
            return FalxNumber(_add(self.value, other.value))

        return super().add(other)

    def minus(self, other: FalxValue) -> FalxValue:
        if isinstance(other, FalxNumber):
            return FalxNumber(_sub(self.value, other.value))
        return super().minus(other)

    def multiply(self, other: FalxValue) -> FalxValue:
        from src.runtime.objects.FalxString import FalxString
        if isinstance(other, FalxNumber):
            return FalxNumber(_mul(self.value, other.value))

        if isinstance(other, FalxString):
            return  FalxString(other.asString()*int(self.value))
        return super().multiply(other)

    def divide(self, other: FalxValue) -> FalxValue:
        if isinstance(other, FalxNumber):
            return FalxNumber(_div(self.value, other.value))
        return super().divide(other)

    def mod(self, other: FalxValue) -> FalxValue:
        if isinstance(other, FalxNumber):
            return FalxNumber(_mod(self.value, other.value))
        return super().mod(other)

    def compareTo(self, other: FalxValue) -> int:
        a: float = self.asFloat()
        b: float = other.asFloat()

        if a < b: return -1
        elif b < a: return 1

        return 0 # TODO: flesh out this to support bit comparisons

    def equalsValue(self, other: FalxValue) -> bool:
        return self.asFloat() == other.asFloat()

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, FalxNumber) and self.value == other.value

    def __hash__(self) -> int:
        return hash(self.value)

    def getTypeName(self) -> str:
        return "number"

# Internal helper methods

def _add(a: int | float, b: int | float) -> int | float:
    if isinstance(a, int) and isinstance(b, int):
        return int(a + b)
    return float(a) + float(b)

def _sub(a: int | float, b: int | float) -> int | float:
    if isinstance(a, int) and isinstance(b, int):
        return int(a - b)
    return float(a) - float(b)

def _mul(a: int | float, b: int | float) -> int | float:
    if isinstance(a, int) and isinstance(b, int):
        return int(a * b)
    return float(a) * float(b)

def _div(a: int | float, b: int | float) -> int | float:
    if isinstance(a, int) and isinstance(b, int):
        return int(a / b)
    return float(a) / float(b)

def _mod(a: int | float, b: int | float) -> int | float:
    if isinstance(a, int) and isinstance(b, int):
        return int(a % b)
    return float(a) % b



