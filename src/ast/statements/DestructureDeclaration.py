from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.ast.expressions.DestructureAssignmentExpression import DestructureAssignmentExpression
from src.ast.expressions.VariableExpression import VariableExpression
from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxValue import FalxValue


class DestructureDeclaration(Statement):

    def __init__(self, location: SourceLocation, targets: list[VariableExpression], initializer: Expression, mutable: bool):
        super().__init__(location)
        self.targets = targets
        self.initializer = initializer
        self.mutable = mutable

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        value: FalxValue = self.initializer.evaluate(interpreter, environment)
        elements: tuple[FalxValue, ...] = DestructureAssignmentExpression.unpack(value, len(self.targets), self.location)

        seen: set[str] = set()
        for target in self.targets:
            if target.name in seen or target.name in environment.variables:
                raise VariableAlreadyExistsException(target.name, target.location)
            seen.add(target.name)

        for target, element in zip(self.targets, elements):
            environment.define(target.name, element, self.mutable)

    def __str__(self):
        return f"({', '.join(target.name for target in self.targets)}) = {self.initializer}"

    def __repr__(self):
        return self.__str__()