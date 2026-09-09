from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue

class IndexExpression(Expression):
    def __init__(self, location: SourceLocation, obj: Expression, index: Expression):
        super().__init__(location)
        self.index:  Expression = index
        self.obj: Expression = obj

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        _obj: FalxValue = self.obj.evaluate(interpreter, environment)
        _idx: FalxValue = self.index.evaluate(interpreter, environment)

        return _obj.index(_idx)

    
