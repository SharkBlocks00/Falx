from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException
from src.types.FalxType import ANY, STRING, VOID, FalxType, FunctionType

BUILTIN_FUNCTIONS: dict[str, FalxType] = {
    "output": FunctionType((ANY,), VOID),
    "typeof": FunctionType((ANY,), STRING),
    "assert": FunctionType((ANY,), VOID),
    "require": FunctionType((STRING,), ANY),
    "request": FunctionType((ANY,), ANY),
    "Exception": FunctionType((ANY,), ANY),
}

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

        return BUILTIN_FUNCTIONS.get(name)
    
    def contains_local(self, name: str) -> bool:
        return name in self.variables

    def child_scope(self) -> "TypeEnvironment":
        return TypeEnvironment(parent=self)