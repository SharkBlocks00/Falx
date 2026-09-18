from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class VariableAlreadyExistsException(FalxRuntimeException):
    def __init__(self, name: str, location: SourceLocation | None = None) -> None:
        super().__init__(f"Variable '{name}' already exists", location)