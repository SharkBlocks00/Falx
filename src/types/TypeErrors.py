from src.ast.SourceLocation import SourceLocation


class TypeCheckError(Exception):
    def __init__(self, message: str, location: SourceLocation | None =None) -> None:
        super().__init__(message)
        self.message = message
        self.location = location

    def __str__(self) -> str:
        if self.location is None:
            return self.message

        return f"{self.location}: {self.message}"