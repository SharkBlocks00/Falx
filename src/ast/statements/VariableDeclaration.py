from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.diagnostics.exceptions.runtime.RecursionDepthExceededException import RecursionDepthExceededException
from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxValue import FalxValue


class VariableDeclaration(Statement):
    def __init__(self, location: SourceLocation, name: str, initializer: Expression, mutable: bool, exported: bool):
        super().__init__(location)
        self.initializer = initializer
        self.mutable = mutable
        self.name = name
        self.exported = exported

    def execute(self, interpreter: Interpreter, environment: Environment, EXPORT_ENVIRONMENT: Environment | None = None) -> None:
        try:
            value: FalxValue = self.initializer.evaluate(interpreter, environment)
            environment.define(self.name, value, self.mutable)
            if EXPORT_ENVIRONMENT is not None and self.exported:
                EXPORT_ENVIRONMENT.define(self.name, value, self.mutable)
        except VariableAlreadyExistsException:
            raise VariableAlreadyExistsException(self.name, self.location)
        except CannotConvertToTypeException as e:
            raise CannotConvertToTypeException(self.name, e.expected).withLocation(self.location)
        except RecursionError:
            raise RecursionDepthExceededException(20, self.location)

    def __str__(self):
        return f"{self.name} = {self.initializer}"

    def __repr__(self):
        return f"{self.name} = {self.initializer}"
