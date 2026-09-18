from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class InvalidArgumentCountException(FalxRuntimeException):
    def __init__(self, count: int | None = None, expected: int | None = None, location: SourceLocation | None = None, message: str | None = None):
        super().__init__(f"Expected {expected} arguments, but got {count}"
                         if not message else message
                         , location)