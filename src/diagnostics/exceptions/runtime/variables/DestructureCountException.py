from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class DestructureCountException(FalxRuntimeException):
    def __init__(self, expected: int, actual: int, location: SourceLocation | None = None) -> None:
        super().__init__(
            f"Cannot destructure a tuple with {actual} element{'' if actual == 1 else 's'} "
            f"into {expected} variable{'' if expected == 1 else 's'}",
            location
        )