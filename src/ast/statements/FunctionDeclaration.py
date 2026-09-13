from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.diagnostics.exceptions.FalxRuntimeException import FalxRuntimeException
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.values.UserFunction import UserFunction


class FunctionDeclaration(Statement):
    def __init__(self, location: SourceLocation, name: str, parameters: list[str], body: list[Statement]):
        super().__init__(location)
        self.name = name
        self.parameters = parameters
        self.body = body

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        try:
            function: UserFunction = UserFunction(self.parameters, self.body, environment)
            environment.define(self.name, function, False)
        except RuntimeError as e:
            raise FalxRuntimeException(str(e), self.location) from None

    def __str__(self) -> str:
        return f"{self.name}({self.parameters}) -> {self.body}"

    def getTypeName(self) -> str:
        return self.name