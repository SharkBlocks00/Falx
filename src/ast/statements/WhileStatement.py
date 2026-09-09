from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.exceptions.BreakException import BreakException
from src.exceptions.ContinueException import ContinueException
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter


class WhileStatement(Statement):
    def __init__(self, location: SourceLocation, condition: Expression, body: list[Statement]):
        super().__init__(location)
        self.condition: Expression = condition
        self.body: list[Statement] = body

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        while self.condition.evaluate(interpreter, environment).asBool():
            try:
                for stmt in self.body:
                    stmt.execute(interpreter, environment)
            except ContinueException:
                continue
            except BreakException:
                break

    
