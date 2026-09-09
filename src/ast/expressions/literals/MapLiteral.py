from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxString import FalxString
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.objects.FalxMap import FalxMap

class MapLiteral(Expression):
    def __init__(self, location: SourceLocation, values: dict[str, Expression]):
        super().__init__(location)
        self.values: dict[str, Expression] = values


    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        elements: dict[FalxValue, FalxValue] = {}

        for key, expr in self.values.items():
            value = expr.evaluate(interpreter, environment)
            elements[FalxString(key)] = value

        return FalxMap(elements)