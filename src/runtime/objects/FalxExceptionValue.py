from src.diagnostics.exceptions.FalxException import FalxException
from src.runtime.objects.FalxValue import FalxValue


class FalxExceptionValue(FalxValue):
    """A Falx value that carries an exception instance."""

    def __init__(self, exception: FalxException):
        super().__init__()
        self.exception = exception

    def __eq__(self, other: object) -> bool:
        return (
            isinstance(other, FalxExceptionValue)
            and type(self.exception) is type(other.exception)
            and self.exception.message == other.exception.message
        )

    def __hash__(self) -> int:
        return hash((type(self.exception), self.exception.message))

    def getTypeName(self) -> str:
        return type(self.exception).__name__

    def __str__(self) -> str:
        return self.exception.message
