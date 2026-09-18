from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class RecursionDepthExceededException(FalxRuntimeException):
    def __init__(self, maxDepth: int, location: SourceLocation | None = None):
        super().__init__(f"Maximum recursion depth of {maxDepth} exceeded", location)
        self.maxDepth = maxDepth