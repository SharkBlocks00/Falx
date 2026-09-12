from __future__ import annotations
from typing import TYPE_CHECKING, Optional

from src.ast.expressions.BinaryExpression import BinaryExpression
from src.tokens.Token import Token

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue


class SetExpression(Expression):
    def __init__(self, location: SourceLocation, obj: Expression, _property: str, value: Expression, operator: Optional[Token] = None):
        super().__init__(location)
        self.obj = obj
        self.property = _property
        self.value = value
        # None for plain obj.prop = value BUT: set to base operator
        # for compound assignment, eg TokenKind.PLUS for +=
        self.operator = operator


    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        falxObject: FalxValue = self.obj.evaluate(interpreter, environment)

        if self.operator is not None:
            currentValue: FalxValue = falxObject.get(self.property)
            rhsValue: FalxValue = self.value.evaluate(interpreter, environment)
            evaluatedValue: FalxValue = BinaryExpression.applyOperator(self.operator.tokenKind, currentValue, rhsValue)
        else:
            evaluatedValue: FalxValue = self.value.evaluate(interpreter, environment)

        falxObject.set(self.property, evaluatedValue)
        return evaluatedValue
    