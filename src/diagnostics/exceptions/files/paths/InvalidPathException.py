from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOErrorException

class InvalidPathException(IOErrorException):
    def __init__(self, path: str, location: SourceLocation | None = None):
        super().__init__(f"Path'{path}' is invalid.", location)
        self.path = path