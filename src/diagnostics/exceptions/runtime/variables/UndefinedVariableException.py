from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class UndefinedVariableException(FalxRuntimeException):
    def __init__(self, name: str, location: SourceLocation | None = None):
        super().__init__(f"undefined variable '{name}'", location)

        self.name = name