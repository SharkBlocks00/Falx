from src.packages.Callable import Callable
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxStruct import FalxStruct
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.FieldDefinition import FieldDefinition
from src.runtime.values.StructDefinition import StructDefinition


class StructConstructor(Callable):
    def __init__(self, definition: StructDefinition):
        super().__init__()
        self.definition = definition

    def arity(self) -> int:
        return len(self.definition.fields)

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        struct: FalxStruct = FalxStruct(self.definition)

        fields: list[FieldDefinition] = self.definition.fields

        env: Environment = Environment(interpreter.environment)

        i = 0
        while i < len(fields):
            field: FieldDefinition = fields[i]

            value: FalxValue

            if i < len(arguments):
                value: FalxValue = arguments[i]
            elif field.defaultValue is not None:
                value: FalxValue = field.defaultValue.evaluate(interpreter, env)
            else:
                value: FalxValue = FalxNull()

            struct[field.name] = value

        return struct

    def __eq__(self, other: object):
        return isinstance(other, StructConstructor) and self.definition == other.definition

    def __hash__(self) -> int:
        return hash(self.definition)