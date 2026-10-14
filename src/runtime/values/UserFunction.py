from src.ast.Statement import Statement
from src.diagnostics.exceptions.runtime.RecursionDepthExceededException import RecursionDepthExceededException
from src.packages.Callable import Callable
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.ParameterDefinition import ParameterDefinition
from src.runtime.values.ReturnException import ReturnException
from src.diagnostics.exceptions.runtime.methods.InvalidArgumentCountException import InvalidArgumentCountException


class UserFunction(Callable):
    """Base class for all user defined functions."""

    def __init__(self, parameters: list[ParameterDefinition], body: list[Statement], closure: Environment):
        """Initialize a user-defined function.

        Args:
            parameters (list[ParameterDefinition]): The list of defined parameters
            body (list[Statement]): The list of statements within the function to be executed
            closure (Environment): The external environment of the function
        """

        super().__init__()
        self.parameters: list[ParameterDefinition] = parameters
        self.body: list[Statement] = body
        self.closure: Environment = closure
        self.isStandardLibrary: bool = closure.isStandardLibrary

    def getTypeName(self) -> str:
        return "function"

    def isStrict(self) -> bool:
        return False

    def arity(self) -> int:
        return len(self.parameters)

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        """Execute the user-defined function.

        Args:
            interpreter (Interpreter): The interpreter to be used to execute the function
            arguments (list[FalxValue]): The list of arguments to be passed to the function

        Raises:
            InvalidArgumentCountException: If the number of arguments passed to the function
                exceeds the maximum number of parameters, or if it is under the number of
                predefined parameters in the function
            RecursionDepthExceededException: If the maximum recursion depth is exceeded

        Returns:
            FalxNull: FalxNull: If there was no earlier return type/statement.
            FalxValue: The value to be returned, using `return`.
        """

        if len(arguments) > len(self.parameters):
            raise InvalidArgumentCountException(len(arguments), len(self.parameters))

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
            else:
                raise InvalidArgumentCountException(len(arguments), sum(parameter.defaultValue is None for parameter in self.parameters))



        try:
            if interpreter.recursionDepth >= interpreter.maxRecursionDepth:
                raise RecursionDepthExceededException(interpreter.maxRecursionDepth)

            interpreter.recursionDepth += 1

            for stmt in self.body:
                stmt.execute(interpreter, local)
        except ReturnException as r:
            return r.value
        finally:
            interpreter.recursionDepth -= 1

        return FalxNull()

    def __str__(self) -> str:
        return f"{self.parameters} -> {self.body}"

    def __repr__(self) -> str:
        return f"{self.parameters} -> {self.body}"

    def __eq__(self, other: object) -> bool:
        return isinstance(other, UserFunction) and self.parameters == other.parameters and self.body == other.body

    def __hash__(self) -> int:
        return hash((self.parameters, self.body))
