from __future__ import annotations

from src.ast.Variable import Variable
from src.runtime.objects.FalxValue import FalxValue

class Environment:
    """Environment class for Falx, nothing should inherit this,
    as many copies of this class can be made as needed
    """

    def __init__(self, parent: Environment = None):
        self.parent: Environment = parent
        self.variables: dict[str, Variable] = {}

    def define(self, name: str, value: FalxValue, mutable: bool = True) -> None:
        if self.variables.get(name) is not None:
            raise RuntimeError(f"Variable '{name}' already exists")

        self.variables[name] = Variable(value, mutable=mutable)

    def assign(self, name: str, value: FalxValue) -> None:
        variable: Variable = self.variables.get(name)

        if variable is not None:
            if not variable.mutable:
                raise RuntimeError(f"Cannot assign to constant '{name}'")
            variable.value = value

        if self.parent is not None:
            self.parent.assign(name, value)
            return

        raise RuntimeError(f"Undefined variable '{name}'")

    def get(self, name: str) -> FalxValue:
        variable: Variable = self.variables.get(name)

        if variable is not None:
            return variable.value

        if self.parent is not None:
            return self.parent.get(name)

        raise RuntimeError(f"Undefined variable '{name}'")


