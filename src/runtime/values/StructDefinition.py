from src.runtime.Environment import Environment
from src.runtime.values.FieldDefinition import FieldDefinition
from src.runtime.values.UserFunction import UserFunction


class StructDefinition:
    """Base struct class for struct definitions"""
    def __init__(self, name: str, fields: list[FieldDefinition], methods: dict[str, UserFunction], closure: Environment):
        self.name = name
        self.fields = fields
        self.methods = methods
        self.closure = closure

