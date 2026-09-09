from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter


class ExpressionStatement(Statement):
    def __init__(self, location: SourceLocation, expression: Expression):
        super().__init__(location)
        self.expression = expression

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        self.expression.evaluate(interpreter, environment)

    def __str__(self):
        return str(self.expression)