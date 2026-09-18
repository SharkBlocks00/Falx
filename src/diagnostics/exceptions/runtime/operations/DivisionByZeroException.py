from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class DivisionByZeroException(FalxRuntimeException):
    def __init__(self, location: SourceLocation):
        super().__init__("cannot divide by zero", location)