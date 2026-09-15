from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException
from src.diagnostics.exceptions.runtime.methods.InvalidArgumentCountException import InvalidArgumentCountException
from src.diagnostics.exceptions.runtime.methods.ObjectNotCallableException import ObjectNotCallableException

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.packages.Callable import Callable
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue
from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation

class FunctionCallExpression(Expression):
    def __init__(self, location: SourceLocation, callee: Expression, arguments: list[Expression]):
        super().__init__(location)
        self.callee: Expression = callee
        self.arguments: list[Expression] = arguments

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        obj: FalxValue = self.callee.evaluate(interpreter, environment)

        if not isinstance(obj, Callable):
            raise ObjectNotCallableException(obj.getTypeName(), self.location)

        args: list[FalxValue] = []

        for arg in self.arguments:
            args.append(arg.evaluate(interpreter, environment))

        if len(args) != obj.arity() and  obj.isStrict():
            raise InvalidArgumentCountException(len(args), obj.arity(), self.location)

        return obj.call(interpreter, args)

    def __str__(self):
        return f"{self.callee} ({self.arguments})"