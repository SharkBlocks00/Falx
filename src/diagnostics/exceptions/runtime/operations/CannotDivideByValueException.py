from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotDivideByValueException(FalxRuntimeException):
    def __init__(self, attemptedValue: str, firstObjectName: str, location: SourceLocation | None = None):
        super().__init__(f"Cannot divide {firstObjectName} by {attemptedValue}.", location)