from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue
from src.tokens.Token import Token
from src.tokens.TokenKind import TokenKind


class LogicalExpression(Expression):
    def __init__(self, location: SourceLocation, left: Expression, operator: Token, right: Expression):
        super().__init__(location)
        self.left: Expression = left
        self.operator: Token = operator
        self.right: Expression = right

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        leftValue: FalxValue = self.left.evaluate(interpreter, environment)

        if self.operator.tokenKind == TokenKind.OR:
            if isTruthy(leftValue):
                return leftValue

            return self.right.evaluate(interpreter, environment)

        if self.operator.tokenKind == TokenKind.AND:
            if not isTruthy(leftValue):
                return leftValue
            return self.right.evaluate(interpreter, environment)

        return leftValue



def isTruthy(value: FalxValue) -> bool:
    return value.asBool()