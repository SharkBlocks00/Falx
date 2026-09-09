from src.ast.Expression import Expression


class FieldDefinition:
    """Class for all fields inside a struct, moreso when they are defined in the base struct"""
    def __init__(self, name: str, defaultValue: Expression):
        self.name = name
        self.defaultValue = defaultValue

    def __str__(self):
        return f"FieldDefinition{{name={self.name}, defaultValue={self.defaultValue}}}"