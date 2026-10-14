from src.runtime.Environment import Environment
from src.runtime.values.FieldDefinition import FieldDefinition
from src.runtime.values.UserFunction import UserFunction


class StructDefinition:
    """Base struct class for struct definitions"""

    def __init__(self, name: str, fields: list[FieldDefinition], methods: dict[str, UserFunction], closure: Environment):
        """Initialize the struct definition.

        Args:
            name (str): The name of the struct
            fields (list[FieldDefinition]): The list of defined fields
            methods (dict[str, UserFunction]): The list of user defined methods within the struct
            closure (Environment): The external environment of the struct
        """

        self.name = name
        self.fields = fields
        self.methods = methods
        self.closure = closure

