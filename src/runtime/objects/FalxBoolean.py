from src.runtime.objects.FalxValue import FalxValue


class FalxBoolean(FalxValue):
    def __init__(self, value: bool):
        super().__init__()
        self.value: bool = value

    def compareTo(self, other: FalxValue) -> int:
        a: float = self.asFloat()
        b: float = other.asFloat()

        return a == b

    def equalsValue(self, other: FalxValue) -> bool:
        return self.asFloat == other.asFloat()

    def asString(self) -> str:
        return str(self.value)

    def asFloat(self) -> float:
        return float(self.value)

    def asInt(self):
        return int(self.value)

    def asBool(self) -> bool:
        return self.value

    def __eq__(self, other: FalxValue) -> bool:
        return isinstance(other, FalxBoolean) and self.value == other.value

    def __hash__(self) -> int:
        return hash(self.value)

    def __str__(self) -> str:
        return str(self.value)

    def __repr__(self) -> str:
        return str(self.value)

    def getTypeName(self) -> str:
        return "boolean"

