from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class InvalidArgumentCountException(FalxRuntimeException):
    def __init__(self, count: int, expected: int, location: SourceLocation | None = None):
        super().__init__(f"Expected {expected} arguments, but got {count}", location)