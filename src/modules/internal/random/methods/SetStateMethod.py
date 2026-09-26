import random

from src.diagnostics.exceptions.runtime.ExpectedValueException import ExpectedValueException
from src.runtime.Interpreter import Interpreter
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxBoolean import FalxBoolean
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxTuple import FalxTuple
from src.runtime.objects.FalxValue import FalxValue

class SetStateMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 1

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        value = arguments[0]

        if not isinstance(value, FalxTuple):
            raise ExpectedValueException(
                f"Cannot set random with '{value.getTypeName()}'"
            )

        random.setstate(_toPythonValue(value))
        return value

    def __eq__(self, other: object) -> bool:
        return isinstance(other, SetStateMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)

def _toPythonValue(value: FalxValue) -> object:
    if isinstance(value, FalxNull):
        return None

    if isinstance(value, FalxNumber):
        return value.asNumber()

    if isinstance(value, FalxTuple):
        return tuple(_toPythonValue(item) for item in value.values)

    raise TypeError(f"Unsupported random state value: {value.getTypeName()}")