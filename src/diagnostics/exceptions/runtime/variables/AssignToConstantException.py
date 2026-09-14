from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class AssignToConstantException(FalxRuntimeException):
    def __init__(self, name: str, location: SourceLocation | None = None) -> None:
        super().__init__(f"Cannot assign to constant '{name}'", location)