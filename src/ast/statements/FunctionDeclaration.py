from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.diagnostics.exceptions.runtime.methods.DuplicateParameterException import DuplicateParameterException
from src.diagnostics.exceptions.runtime.variables.FunctionAlreadyExistsException import FunctionAlreadyExistsException
from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException
from src.packages.NativeFunction import NativeFunction
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.ParameterDefinition import ParameterDefinition
from src.runtime.values.UserFunction import UserFunction


class FunctionDeclaration(Statement):
    def __init__(self, location: SourceLocation, name: str, parameters: list[ParameterDefinition], body: list[Statement]):
        super().__init__(location)
        self.name = name
        self.parameters = parameters
        self.body = body

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:

        seen: set[str] = set()

        for parameter in self.parameters:
            if parameter.name in seen:
                raise DuplicateParameterException(parameter.name, self.location)

            seen.add(parameter.name)

        try:
            function: UserFunction = UserFunction(self.parameters, self.body, environment)
            environment.define(self.name, function, False)
        except VariableAlreadyExistsException:
            existing: FalxValue = environment.getLocal(self.name)
            if isinstance(existing, (UserFunction, NativeFunction)):
                raise FunctionAlreadyExistsException(self.name, self.location)
            raise VariableAlreadyExistsException(self.name, self.location)

    def __str__(self) -> str:
        return f"{self.name}({self.parameters}) -> {self.body}"

    def getTypeName(self) -> str:
        return self.name