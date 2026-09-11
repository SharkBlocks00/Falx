from __future__ import annotations
from typing import TYPE_CHECKING

from src.ast.Expression import Expression
from src.runtime.Environment import Environment
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue
from src.tokens.Token import Token

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class NullLiteral(Expression):
    def __init__(self, token: Token):
        super().__init__(token.location)
        self.value = token.lexeme

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        return FalxNull()