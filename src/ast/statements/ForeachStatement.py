from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.exceptions.BreakException import BreakException
from src.exceptions.ContinueException import ContinueException
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxIterable import FalxIterable
from src.runtime.objects.FalxValue import FalxValue


class ForeachStatement(Statement):
    def __init__(self, location: SourceLocation, variable: str, iterable: Expression, body: list[Statement]):
        super().__init__(location)
        self.variable = variable
        self.iterable = iterable
        self.body = body

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        value: FalxValue = self.iterable.evaluate(interpreter, environment)

        if not isinstance(value, FalxIterable):
            raise RuntimeError("Object is not iterable")

        for element in value.iterate():
            # use an isolated environment for the variable being used to iterate
            loopEnv: Environment = Environment(environment)
            loopEnv.define(self.variable, element, False)

            for stmt in self.body:
                try:
                    stmt.execute(interpreter, environment)
                except BreakException:
                    break
                except ContinueException:
                    continue
                    