from src.ast.Statement import Statement
from src.packages.Callable import Callable
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.ParameterDefinition import ParameterDefinition
from src.runtime.values.ReturnException import ReturnException


class UserFunction(Callable):
    """Base class for all user defined functions."""
    def __init__(self, parameters: list[ParameterDefinition], body: list[Statement], closure: Environment):
        super().__init__()
        self.parameters: list[ParameterDefinition] = parameters
        self.body: list[Statement] = body
        self.closure: Environment = closure

    def isStrict(self) -> bool:
        return False

    def arity(self) -> int:
        return len(self.parameters)

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        local: Environment = Environment(self.closure)

        for i in range(len(self.parameters)):
            argExists: bool = False
            try:
                arguments[i]
                argExists = True
            except IndexError:
                argExists = False

            if self.parameters[i].defaultValue is not None and not argExists:
                local.define(self.parameters[i].name, self.parameters[i].defaultValue.evaluate(interpreter,local), True)
            elif argExists:
                local.define(self.parameters[i].name, arguments[i], True)



        try:
            for stmt in self.body:
                stmt.execute(interpreter, local)
        except ReturnException as r:
            return r.value

        return FalxNull()

    def __str__(self) -> str:
        return f"{self.parameters} -> {self.body}"

    def __repr__(self) -> str:
        return f"{self.parameters} -> {self.body}"

    def __eq__(self, other: object) -> bool:
        return isinstance(other, UserFunction) and self.parameters == other.parameters and self.body == other.body

    def __hash__(self) -> int:
        return hash((self.parameters, self.body))
