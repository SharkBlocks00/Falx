from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.FalxRuntimeException import FalxRuntimeException


class DivisionByZeroException(FalxRuntimeException):
    def __init__(self, location: SourceLocation):
        super().__init__(f"cannot divide by zero", location)