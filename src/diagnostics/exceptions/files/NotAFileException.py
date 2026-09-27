from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOErrorException

class NotAFileException(IOErrorException):
    def __init__(self, path: str, location: SourceLocation | None = None):
        super().__init__(f"'{path}' is not a file.", location)
        self.path = path