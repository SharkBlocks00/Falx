from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.operations.CannotAddWithValueException import CannotAddWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotCompareToException import CannotCompareToException
from src.diagnostics.exceptions.runtime.operations.CannotDivideByValueException import CannotDivideByValueException
from src.diagnostics.exceptions.runtime.operations.CannotEvaluateValueException import CannotEvaluateValueException
from src.diagnostics.exceptions.runtime.operations.CannotMinusFromValueException import CannotMinusFromValueException
from src.diagnostics.exceptions.runtime.operations.CannotModWithValueException import CannotModWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotMultiplyByValueException import CannotMultiplyByValueException
from src.diagnostics.exceptions.runtime.operations.DivisionByZeroException import DivisionByZeroException
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxBoolean import FalxBoolean
from src.runtime.objects.FalxValue import FalxValue
from src.tokens.Token import Token
from src.tokens.TokenKind import TokenKind


class BinaryExpression(Expression):
    def __init__(self, location: SourceLocation, left: Expression, operator: Token, right: Expression):
        super().__init__(location)
        self.left: Expression = left
        self.operator: Token = operator
        self.right: Expression = right

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        leftValue: FalxValue = self.left.evaluate(interpreter, environment)
        rightValue: FalxValue = self.right.evaluate(interpreter, environment)

        try:
             return BinaryExpression.applyOperator(self.operator.tokenKind, leftValue, rightValue)
        except CannotMultiplyByValueException:
            raise CannotMultiplyByValueException(rightValue.asString(), leftValue.asString(), self.location) from None
        except CannotDivideByValueException:
            raise CannotDivideByValueException(rightValue.asString(), leftValue.asString(), self.location) from None
        except CannotAddWithValueException:
            raise CannotAddWithValueException(leftValue.asString(), rightValue.asString(), self.location) from None
        except CannotModWithValueException:
            raise CannotModWithValueException(leftValue.asString(), rightValue.asString(), self.location) from None
        except CannotEvaluateValueException:
            raise CannotEvaluateValueException(leftValue.asString(), rightValue.asString()).withLocation(self.location)
        except CannotMinusFromValueException:
            raise CannotMinusFromValueException(leftValue.asString(), rightValue.asString(), self.location) from None
        except CannotCompareToException as e:
            raise e.withLocation(self.location) from e
        except RuntimeError as e:
            raise FalxRuntimeException(str(e), self.location) from None
        except ZeroDivisionError:
            raise DivisionByZeroException(self.location) from None


    @staticmethod
    def applyOperator(operatorKind: TokenKind, leftValue: FalxValue, rightValue: FalxValue) -> FalxValue:
        match operatorKind:
            case TokenKind.PLUS: return leftValue.add(rightValue)
            case TokenKind.MINUS: return leftValue.minus(rightValue)
            case TokenKind.STAR: return leftValue.multiply(rightValue)
            case TokenKind.SLASH: return leftValue.divide(rightValue)
            case TokenKind.PERCENT: return leftValue.mod(rightValue)

            # Assignment operators
            case TokenKind.GREATER: return FalxBoolean(leftValue.compareTo(rightValue) > 0)
            case TokenKind.GREATER_EQUAL: return FalxBoolean(leftValue.compareTo(rightValue) >= 0)
            case TokenKind.LESS: return FalxBoolean(leftValue.compareTo(rightValue) < 0)
            case TokenKind.LESS_EQUAL: return FalxBoolean(leftValue.compareTo(rightValue) <= 0)
            case TokenKind.EQUAL_EQUAL: return FalxBoolean(leftValue.equalsValue(rightValue))
            case TokenKind.BANG_EQUAL: return FalxBoolean(not leftValue.equalsValue(rightValue))

            case _: raise CannotEvaluateValueException(leftValue.asString(), rightValue.asString())

    def __str__(self) -> str:
        return f"{self.left.__str__} {self.operator.tokenKind.__str__()} {self.right.__str__()}"