from __future__ import annotations
from typing import TYPE_CHECKING

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxTuple import FalxTuple
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from abc import ABC

class TupleLiteral(Expression, ABC):
    def __init__(self, location: SourceLocation, elements: list[Expression]):
        super().__init__(location)
        self.elements = elements

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        values: list[FalxValue] = []

        for expr in self.elements:
            values.append(expr.evaluate(interpreter, environment))

        return FalxTuple(values)