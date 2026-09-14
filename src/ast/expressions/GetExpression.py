from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException
from src.diagnostics.exceptions.runtime.properties.CannotAccessPropertyException import CannotAccessPropertyException

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue

class GetExpression(Expression):
    def __init__(self, location: SourceLocation, obj: Expression, _property: str):
        super().__init__(location)
        self.obj: Expression = obj
        self._property: str = _property

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        value: FalxValue = self.obj.evaluate(interpreter, environment)
        try:
            return value.get(self._property)
        except CannotAccessPropertyException:
            raise CannotAccessPropertyException(f"'{self.obj}' has no property '{self._property}'", self.location)
        except RuntimeError as e:
            raise FalxRuntimeException(str(e), self.location) from None

    def __str__(self):
        return f"GetExpression({self.obj}, {self._property})"

    def __repr__(self):
        return f"GetExpression({self.obj}, {self._property})"
