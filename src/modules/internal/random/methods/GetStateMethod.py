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

class GetStateMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 0

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        return _toFalxValue(random.getstate())

    def __eq__(self, other: object) -> bool:
        return isinstance(other, GetStateMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)

def _toFalxValue(value: object) -> FalxValue:
    if value is None:
        return FalxNull()

    if isinstance(value, bool):
        return FalxBoolean(value)
    if isinstance(value, int | float):
        return FalxNumber(value)
    if isinstance(value, tuple):
        return FalxTuple(_toFalxValue(item) for item in value)

    raise ExpectedValueException(f"Unsupported random state value: {type(value)}")