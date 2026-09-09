from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.ReturnException import ReturnException


class ReturnStatement(Statement):
    def __init__(self, location: SourceLocation, value: Expression):
        super().__init__(location)
        self.value = value

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        obj: FalxValue = FalxNull()
        if self.value is not None:
            obj = self.value.evaluate(interpreter, environment)

        ReturnException(obj) # by doing this we can intercept the ReturnException.value and get what was returned

    def __str__(self) -> str:
        return f"return {self.value}"