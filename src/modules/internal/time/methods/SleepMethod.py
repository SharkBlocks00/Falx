import time

from src.diagnostics.exceptions.runtime.typing.UnexpectedTypeException import UnexpectedTypeException
from src.runtime.Interpreter import Interpreter
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue

class SleepMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 1

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        val: FalxValue = arguments[0]
        if not isinstance(val, FalxNumber):
            raise UnexpectedTypeException("number", val.getTypeName())
        time.sleep(val.asNumber())
        return FalxNull()

    def __eq__(self, other: object) -> bool:
        return isinstance(other, SleepMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)
