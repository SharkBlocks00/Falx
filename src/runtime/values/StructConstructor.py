from src.diagnostics.exceptions.runtime.methods.InvalidArgumentCountException import InvalidArgumentCountException
from src.packages.Callable import Callable
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxStruct import FalxStruct
from src.runtime.objects.FalxValue import FalxValue
from src.runtime.values.FieldDefinition import FieldDefinition
from src.runtime.values.StructDefinition import StructDefinition


class StructConstructor(Callable):
    """Class representing a struct constructor."""

    def __init__(self, definition: StructDefinition):
        """Initialize the constructor and set its definition.

        Args:
            definition (StructDefinition): the definition of the struct
        """
        super().__init__()
        self.definition = definition

    def isStrict(self) -> bool:
        return False

    def arity(self) -> int:
        return len(self.definition.fields)

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        """Create a FalxStruct from the definition and the given arguments.

        Each argument sets a field's value inside the struct, will default to null if
        an argument is not given for a certain field.

        Args:
            interpreter (Interpreter): the interpreter class to use
            arguments (list[FalxValue]): the arguments to set the struct's values to

        Raises:
            InvalidArgumentCountException: If the provided argument count does not
                either match the number of required fields, or if there are more
                arguments passed than the total number of fields.

        Returns:
            FalxStruct: The created FalxStruct
        """

        requiredFields: int = len([field for field in self.definition.fields if field.defaultValue is None])

        if len(arguments) != requiredFields and len(arguments) > len(self.definition.fields):
            raise InvalidArgumentCountException(len(arguments), len(self.definition.fields))

        struct: FalxStruct = FalxStruct(self.definition)

        fields: list[FieldDefinition] = self.definition.fields

        i = 0
        while i < len(fields):
            field: FieldDefinition = fields[i]

            value: FalxValue

            if i < len(arguments):
                value: FalxValue = arguments[i]
            elif field.defaultValue is not None:
                value: FalxValue = field.defaultValue.evaluate(interpreter, Environment(self.definition.closure))
            else:
                value: FalxValue = FalxNull()

            struct.set(field.name, value)
            i += 1

        return struct

    def __eq__(self, other: object):
        return isinstance(other, StructConstructor) and self.definition == other.definition

    def __hash__(self) -> int:
        return hash(self.definition)