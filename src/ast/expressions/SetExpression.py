from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue


class SetExpression(Expression):
    def __init__(self, location: SourceLocation, obj: Expression, _property: str, value: Expression):
        super().__init__(location)
        self.obj = obj
        self.property = _property
        self.value = value


    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        falxObject: FalxValue = self.obj.evaluate(interpreter, environment)
        evaluatedValue: FalxValue = self.value.evaluate(interpreter, environment)

        falxObject.set(self.property, evaluatedValue)
        return evaluatedValue
    