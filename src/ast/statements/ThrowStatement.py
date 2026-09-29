from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.diagnostics.exceptions.runtime.ExpectedValueException import ExpectedValueException
from src.runtime.Environment import Environment
from src.runtime.objects.FalxExceptionValue import FalxExceptionValue
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxThrowable import FalxThrowable
from src.runtime.objects.FalxValue import FalxValue


class ThrowStatement(Statement):
    def __init__(self, location: SourceLocation, value: Expression):
        super().__init__(location)
        self.value = value

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        throwable: FalxValue = self.value.evaluate(interpreter, environment)
        if isinstance(throwable, FalxExceptionValue):
            raise throwable.exception.withLocation(self.location)
        if not isinstance(throwable, FalxThrowable):
            raise ExpectedValueException("throw expects an Exception value", self.location)

        raise throwable.withLocation(self.location)

    def __str__(self) -> str:
        return f"throw {self.value}"
