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

class IndexSetExpression(Expression):
    def __init__(self, location: SourceLocation, obj: Expression, index: Expression, value: Expression, operator: Optional[Token] = None) -> None:
        super().__init__(location)
        self.obj = obj
        self.index = index
        self.value = value
        self.operator = operator

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        falxObject: FalxValue = self.obj.evaluate(interpreter, environment)
        evaluatedIndex: FalxValue = self.index.evaluate(interpreter, environment)

        if self.operator is not None:
            currentValue: FalxValue = falxObject.index(evaluatedIndex)
            rhsValue: FalxValue = self.value.evaluate(interpreter, environment)
            evaluatedValue: FalxValue = BinaryExpression.applyOperator(self.operator.tokenKind, currentValue, rhsValue)
        else:
            evaluatedValue: FalxValue = self.value.evaluate(interpreter, environment)

        falxObject.indexAssign(evaluatedIndex, evaluatedValue)
        return evaluatedValue

    def __str__(self):
        return f"IndexSetExpression({self.obj}, {self.index}, {self.value})"

    def __repr__(self):
        return self.__str__()