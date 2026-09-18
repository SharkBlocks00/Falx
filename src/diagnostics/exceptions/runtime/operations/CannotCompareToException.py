from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotCompareToException(FalxRuntimeException):
    def __init__(self, leftValue: str, rightValue: str, location: SourceLocation | None = None):
        super().__init__(f"Cannot compare '{leftValue}' with '{rightValue}'")