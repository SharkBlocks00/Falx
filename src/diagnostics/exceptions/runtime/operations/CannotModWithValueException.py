from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotModWithValueException(FalxRuntimeException):
    def __init__(self, leftValue: str, rightValue: str, location: SourceLocation | None = None):
        super().__init__(f"Cannot mod {leftValue} by {rightValue}", location)