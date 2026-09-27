from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOErrorException

class FileClosedException(IOErrorException):
    def __init__(self, path: str, location: SourceLocation | None = None):
        super().__init__(f"File '{path}' has already been closed.", location)
        self.path = path