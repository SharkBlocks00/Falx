from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.StructDefinition import StructDefinition


class FalxStruct(FalxValue):
    def __init__(self, definition: StructDefinition):
        super().__init__()
        self.definition = definition
        self.values: dict[str, FalxValue] = {}

        for field in definition.fields:
            self.values[field.name] = FalxNull()


    def get(self, name: str) -> FalxValue:
        if name not in self.values:
            return super().get(name)
        return self.values[name]

    def set(self, name: str, value: FalxValue) -> None:
        if name not in self.values:
            raise RuntimeError(f"Struct '{self.definition.name}' has no field '{name}'")
        self.values[name] = value

    def __eq__(self, other: object) -> bool:
        return isinstance(other, FalxStruct) and self.definition == other.definition and self.values == other.values

    def __hash__(self) -> int:
        return hash(self.definition)

    def getTypeName(self) -> str:
        return self.definition.name

    def __str__(self) -> str:
        return self.definition.name + str(self.values)

