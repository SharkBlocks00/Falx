from src.ast.Expression import Expression


class ParameterDefinition:
    """Class used to represent a parameter when it is defined in a function definition."""

    def __init__(self, name: str, defaultValue: Expression | None = None):
        self.defaultValue = defaultValue
        self.name = name

    def __str__(self):
        return f"ParameterDefinition{{name={self.name}, defaultValue={self.defaultValue}}}"