from src.runtime.objects.FalxValue import FalxValue

class Variable(FalxValue):
    def __init__(self, value: FalxValue, mutable: bool = True):
        super().__init__()
        self.value = value
        self.mutable = mutable

    def __eq__(self, other) -> bool:
        return isinstance(other, Variable) and self.value == other.value and self.mutable == other.mutable

    def __hash__(self) -> int:
        return hash(self.value)

    def __str__(self):
        return str(self.value)

    def __repr__(self):
        return str(self.value)