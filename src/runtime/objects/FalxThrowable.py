from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.FalxException import FalxException
from src.runtime.objects.FalxValue import FalxValue


class FalxThrowable(FalxException, FalxValue):
    """A Falx value that can be propagated by a `throw` statement."""

    def __init__(self, message: str, location: SourceLocation | None = None):
        FalxException.__init__(self, message, location)
        FalxValue.__init__(self)

    def __eq__(self, other: object) -> bool:
        return isinstance(other, FalxThrowable) and self.message == other.message

    def __hash__(self) -> int:
        return hash((FalxThrowable, self.message))

    def getTypeName(self) -> str:
        return "Exception"

    def __str__(self) -> str:
        return self.message
