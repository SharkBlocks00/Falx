from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class ImmutableValueException(FalxRuntimeException):
    def __init__(self, typeName: str, location: SourceLocation | None = None) -> None:
        super().__init__(f"Cannot modify an immutable {typeName}", location)