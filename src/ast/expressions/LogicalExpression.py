from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.diagnostics.exceptions.runtime.operations.CannotEvaluateValueException import CannotEvaluateValueException

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

        try:
            if self.operator.tokenKind == TokenKind.OR and isTruthy(leftValue):
                return leftValue

            if self.operator.tokenKind == TokenKind.AND and not isTruthy(leftValue):
                return leftValue

        except CannotConvertToTypeException:
            raise CannotEvaluateValueException(
                leftValue.asString(),
                self.operator.tokenKind.name
            ).withLocation(self.location)

        return self.right.evaluate(interpreter, environment)


def isTruthy(value: FalxValue) -> bool:
    return value.asBool()