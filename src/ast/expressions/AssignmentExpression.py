from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue


class AssignmentExpression(Expression):
    def __init__(self, location: SourceLocation, name: str, value: Expression):
        super().__init__(location)
        self.name = name
        self.value = value

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        newValue: FalxValue = self.value.evaluate(interpreter, environment)
        try:
            environment.assign(self.name, newValue)
        except FalxRuntimeException as e:
            raise e.withLocation(self.location)
        return newValue