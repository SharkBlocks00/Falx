from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class UnexpectedTypeException(FalxRuntimeException):
    def __init__(self, expected: str, given: str, location: SourceLocation | None = None):
        super().__init__(f"Expected {expected}, but got '{given}'", location)