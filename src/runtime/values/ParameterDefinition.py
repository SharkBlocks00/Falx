from src.ast.Expression import Expression


class ParameterDefinition:
    def __init__(self, name: str, defaultValue: Expression | None = None, type_annotation: str | None = None):
        self.defaultValue = defaultValue
        self.name = name
        self.type_annotation = type_annotation

    def __str__(self):
        return f"ParameterDefinition{{name={self.name}, defaultValue={self.defaultValue}, type={self.type_annotation}}}"