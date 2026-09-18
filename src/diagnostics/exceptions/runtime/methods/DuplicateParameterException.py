from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class DuplicateParameterException(FalxRuntimeException):
    def __init__(self, parameter: str, location: SourceLocation | None = None):
        super().__init__(f"Duplicate parameter '{parameter}'", location)