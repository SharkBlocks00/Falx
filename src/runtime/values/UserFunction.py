from src.ast.Statement import Statement
from src.packages.Callable import Callable
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.ReturnException import ReturnException


class UserFunction(Callable):
    """Base class for all user defined functions."""
    def __init__(self, parameters: list[str], body: list[Statement], closure: Environment):
        super().__init__()
        self.parameters: list[str] = parameters
        self.body: list[Statement] = body
        self.closure: Environment = closure

    def arity(self) -> int:
        return len(self.parameters)

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        local: Environment = Environment(self.closure)

        for i in range(len(self.parameters)):
            local.define(self.parameters[i], arguments[i], True)


        try:
            for stmt in self.body:
                stmt.execute(interpreter, local)
        except ReturnException as r:
            return r.value

        return FalxNull()

    def __eq__(self, other: object) -> bool:
        return isinstance(other, UserFunction) and self.parameters == other.parameters and self.body == other.body

    def __hash__(self) -> int:
        return hash((self.parameters, self.body))
