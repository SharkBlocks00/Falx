from src.ast.SourceLocation import SourceLocation


class FalxException(Exception):
    def __init__(self, message: str, location: SourceLocation | None = None, cause: "FalxException | None" = None):
        super().__init__(message)
        self.message = message
        self.location = location
        self.cause = cause

    def withLocation(self, location: SourceLocation) -> "FalxException":
        if self.location is None:
            self.location = location
        return self