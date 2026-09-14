from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotAddWithValueException(FalxRuntimeException):
    def __init__(self, leftValue: str, rightValue: str, location: SourceLocation | None = None) -> None:
        super().__init__(f"Cannot add {rightValue} to {leftValue}", location)