from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class ObjectNotIterableException(FalxRuntimeException):
    def __init__(self, obj: str, location: SourceLocation | None = None) -> None:
        super().__init__(f"'{obj}' is not iterable", location)