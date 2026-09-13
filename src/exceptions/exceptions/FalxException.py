from src.ast.SourceLocation import SourceLocation


class FalxException(Exception):
    def __init__(self, message: str, location: SourceLocation | None = None):
        super().__init__(message)
        self.message = message
        self.location = location