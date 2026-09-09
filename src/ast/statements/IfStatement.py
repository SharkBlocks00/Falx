from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxBoolean import FalxBoolean
from src.runtime.objects.FalxValue import FalxValue


class IfStatement(Statement):
    def __init__(self, location: SourceLocation, condition: Expression, body: list[Statement], elseBody: Statement) -> None:
        super().__init__(location)
        self.condition = condition
        self.body = body
        self.elseBody = elseBody

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        if isTruthy(self.condition.evaluate(interpreter, environment)):
            for stmt in self.body:
                stmt.execute(interpreter, environment)

        elif self.elseBody is not None:
            self.elseBody.execute(interpreter, environment)

def isTruthy(value: FalxValue) -> bool:
    if isinstance(value, FalxBoolean):
        return value.asBool()
    return False