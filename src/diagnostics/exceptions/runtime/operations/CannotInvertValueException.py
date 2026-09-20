from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotInvertValueException(FalxRuntimeException):
    def __init__(self, value: str, location: SourceLocation | None = None):
        super().__init__(f"Cannot invert '{value}'", location)