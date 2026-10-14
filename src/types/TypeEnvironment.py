from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException
from src.types.FalxType import FalxType

class TypeEnvironment:
    def __init__(self, parent: "TypeEnvironment | None" = None,) -> None:
        self.parent = parent
        self.variables: dict[str, FalxType] = {}

    def define(self, name: str, variable_type: FalxType) -> None:
        if name in self.variables:
            raise VariableAlreadyExistsException(name)

        self.variables[name] = variable_type

    def resolve(self, name: str) -> FalxType | None:
        if name in self.variables:
            return self.variables[name]

        if self.parent is not None:
            return self.parent.resolve(name)

        return None
    
    def contains_local(self, name: str) -> bool:
        return name in self.variables

    def child_scope(self) -> "TypeEnvironment":
        return TypeEnvironment(parent=self)