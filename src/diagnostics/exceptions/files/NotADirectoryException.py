from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOError

class NotADirectoryException(IOError):
    def __init__(self, path: str, location: SourceLocation | None = None):
        super().__init__(f"'{path}' is not a directory.", location)
        self.path = path