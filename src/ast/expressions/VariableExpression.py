from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue


class VariableExpression(Expression):
    def __init__(self, location: SourceLocation, name: str):
        super().__init__(location)
        self.name = name

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        return environment.get(self.name)

    def __str__(self):
        return self.name

    def __repr__(self):
        return self.name