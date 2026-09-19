from __future__ import annotations
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException
from src.diagnostics.exceptions.runtime.typing.UnexpectedTypeException import UnexpectedTypeException
from src.diagnostics.exceptions.runtime.variables.DestructureCountException import DestructureCountException

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.expressions.VariableExpression import VariableExpression
from src.runtime.Environment import Environment
from src.runtime.objects.FalxTuple import FalxTuple
from src.runtime.objects.FalxValue import FalxValue


class DestructureAssignmentExpression(Expression):

    def __init__(self, location: SourceLocation, targets: list[VariableExpression], value: Expression):
        super().__init__(location)
        self.targets = targets
        self.value = value

    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        value: FalxValue = self.value.evaluate(interpreter, environment)
        elements: tuple[FalxValue, ...] = DestructureAssignmentExpression.unpack(value, len(self.targets), self.location)

        for target in self.targets:
            try:
                environment.checkAssignable(target.name)
            except FalxRuntimeException as e:
                raise e.withLocation(target.location)

        for target, element in zip(self.targets, elements):
            environment.assign(target.name, element)

        return value

    @staticmethod
    def unpack(value: FalxValue, count: int, location: SourceLocation) -> tuple[FalxValue, ...]:
        """Returns the elements of the tuple, provided that `value` is a tuple that has exactly `count` elements"""
        if not isinstance(value, FalxTuple):
            raise UnexpectedTypeException("tuple", value.getTypeName(), location)

        if len(value.values) != count:
            raise DestructureCountException(count, len(value.values), location)

        return value.values

    def __str__(self):
        return f"({', '.join(target.name for target in self.targets)}) = {self.value}"

    def __repr__(self):
        return self.__str__()