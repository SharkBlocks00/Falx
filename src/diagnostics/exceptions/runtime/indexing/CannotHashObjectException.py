from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotHashObjectException(FalxRuntimeException):
    def __init__(self, obj: str, _type: str, location: SourceLocation | None = None) -> None:
        super().__init__(f"Cannot hash {_type} with value of '{obj}'", location)