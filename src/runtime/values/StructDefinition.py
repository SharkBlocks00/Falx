from src.runtime.values.FieldDefinition import FieldDefinition


class StructDefinition:
    """Base struct class for struct definitions"""
    def __init__(self, name: str, fields: list[FieldDefinition]):
        self.name = name
        self.fields = fields

