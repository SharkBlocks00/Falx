from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class ObjectNotCallableException(FalxRuntimeException):
    def __init__(self, obj: str, location: SourceLocation | None = None):
        super().__init__(f"'{obj}' is not callable", location)