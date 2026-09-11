from src.ast.statements.FunctionDeclaration import FunctionDeclaration
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.StructDefinition import StructDefinition
from src.runtime.values.UserFunction import UserFunction


class FalxStruct(FalxValue):
    def __init__(self, definition: StructDefinition):
        super().__init__()
        self.definition = definition

        from src.runtime.Environment import Environment
        self.env = Environment(definition.closure)

        for field in definition.fields:
            self.env.define(field.name, FalxNull())

        self.methods: dict[str, UserFunction] = {}
        for name, method in definition.methods.items():
            boundMethod = UserFunction(method.parameters, method.body, self.env)
            self.methods[name] = boundMethod


    def get(self, name: str) -> FalxValue:
        if name in self.methods:
            return self.methods[name]
        try:
            return self.env.get(name)
        except RuntimeError:
            return super().get(name)

    def set(self, name: str, value: FalxValue) -> None:
        try:
            self.env.assign(name, value)
        except RuntimeError:
            raise RuntimeError(f"Struct '{self.definition.name}' has no field '{name}'")

    def __eq__(self, other: object) -> bool:
        return isinstance(other, FalxStruct) and self.definition == other.definition and self.env.variables == other.env.variables

    def __hash__(self) -> int:
        return hash(self.definition)

    def getTypeName(self) -> str:
        return self.definition.name

    def __str__(self) -> str:
        return self.definition.name + str(self.env.variables)

    def __repr__(self) -> str:
        return self.definition.name + str(self.env.variables)

