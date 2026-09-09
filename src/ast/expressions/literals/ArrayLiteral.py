from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from abc import ABC

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment

from src.runtime.objects.FalxValue import FalxValue
from src.runtime.objects.FalxArray import FalxArray

class ArrayLiteral(Expression, ABC):
    def __init__(self, location: SourceLocation, elements: list[Expression]):
        super().__init__(location)
        self._elements = elements

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        values: list[FalxValue] = []

        for expr in self._elements:
            values.append(expr.evaluate(interpreter, environment))

        return FalxArray(values)