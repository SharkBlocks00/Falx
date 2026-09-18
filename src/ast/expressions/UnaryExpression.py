from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxBoolean import FalxBoolean
from src.runtime.objects.FalxNumber import FalxNumber
from src.runtime.objects.FalxValue import FalxValue
from src.tokens.Token import Token
from src.tokens.TokenKind import TokenKind


class UnaryExpression(Expression):
    def __init__(self, location: SourceLocation, operator: Token, expression: Expression):
        super().__init__(location)
        self.operator: Token = operator
        self.expression: Expression = expression

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        try:
            value: FalxValue = self.expression.evaluate(interpreter, environment)

            match self.operator.tokenKind:
                case TokenKind.MINUS: return FalxNumber(-value.asNumber())
                case TokenKind.BANG: return FalxBoolean(not value.asBool())
                case _: return value
        except RuntimeError as e:
            raise FalxRuntimeException(str(e), self.location) from None