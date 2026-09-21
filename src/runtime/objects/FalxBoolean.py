from src.runtime.objects.FalxValue import FalxValue


class FalxBoolean(FalxValue):
    def __init__(self, value: bool):
        super().__init__()
        self.value: bool = value


    def equalsValue(self, other: FalxValue) -> bool:
        if not isinstance(other, FalxBoolean):
            return False
        return self.value == other.value

    def asString(self) -> str:
        return self.__str__()

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
        return "true" if self.value else "false"

    def __repr__(self) -> str:
        return self.__str__()

    def getTypeName(self) -> str:
        return "boolean"

