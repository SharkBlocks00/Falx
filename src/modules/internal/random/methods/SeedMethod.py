import random

from src.diagnostics.exceptions.runtime.ExpectedValueException import ExpectedValueException
from src.runtime.Interpreter import Interpreter
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxValue import FalxValue

class SeedMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 1

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        value = arguments[0]

        if value is FalxNull:
            random.seed(None)
        elif isinstance(value, FalxNumber):
            random.seed(value.asNumber())
        elif isinstance(value, FalxString):
            random.seed(value.asString())
        else:
            raise ExpectedValueException(f"Cannot set seed with '{value.getTypeName()}'")

        return FalxNull()

    def __eq__(self, other: object) -> bool:
        return isinstance(other, SeedMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)