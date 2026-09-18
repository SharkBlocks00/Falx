from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class AssertionFailedException(FalxRuntimeException):
    def __init__(self, message: str | None = None, location: SourceLocation | None = None):
        super().__init__(f"Assertion failed{':' and f' {message}' if message else ''}", location)