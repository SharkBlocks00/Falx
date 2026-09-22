from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException
from src.diagnostics.exceptions.runtime.indexing.ArrayIndexInvalidException import ArrayIndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.InvalidIndexTypeException import InvalidIndexTypeException
from src.diagnostics.exceptions.runtime.indexing.ObjectIsNotIndexableException import ObjectIsNotIndexableException
from src.diagnostics.exceptions.runtime.indexing.StringIndexInvalidException import StringIndexInvalidException

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

        try:
            return _obj.index(_idx)
        except ArrayIndexInvalidException:
            raise ArrayIndexInvalidException(_idx.asInt(), len(_obj.asString())).withLocation(self.location)
        except StringIndexInvalidException:
            raise StringIndexInvalidException(_idx.asInt(), len(_obj.asString())).withLocation(self.location)
        except InvalidIndexTypeException as e:
            raise InvalidIndexTypeException(e.__str__(), self.location)
        except ObjectIsNotIndexableException as e:
            raise e.withLocation(self.location)
    
