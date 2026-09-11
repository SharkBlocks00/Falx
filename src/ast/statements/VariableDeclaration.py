from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxValue import FalxValue


class VariableDeclaration(Statement):
    def __init__(self, location: SourceLocation, name: str, initializer: Expression, mutable: bool):
        super().__init__(location)
        self.initializer = initializer
        self.mutable = mutable
        self.name = name

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        value: FalxValue = self.initializer.evaluate(interpreter, environment)
        environment.define(self.name, value, self.mutable)

    def __str__(self):
        return f"{self.name} = {self.initializer}"

    def __repr__(self):
        return f"{self.name} = {self.initializer}"
