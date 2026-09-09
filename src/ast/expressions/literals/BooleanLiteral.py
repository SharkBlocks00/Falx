from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.runtime.Environment import Environment
from src.runtime.objects.FalxBoolean import FalxBoolean
from src.runtime.objects.FalxValue import FalxValue
from src.tokens.Token import Token


class BooleanLiteral(Expression):
    def __init__(self, token: Token):
        super().__init__(token.location)
        self.value: bool = bool(token.literal)

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        return FalxBoolean(self.value)

    def __str__(self):
        return str(self.value)