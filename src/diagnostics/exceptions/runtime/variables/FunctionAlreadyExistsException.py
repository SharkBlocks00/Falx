from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class FunctionAlreadyExistsException(FalxRuntimeException):
    def __init__(self, name: str, location: SourceLocation | None = None):
        super().__init__(f"Function '{name}' already exists", location)