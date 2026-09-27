from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOError

class InvalidFileModeException(IOError):
    def __init__(self, mode: str, location: SourceLocation | None = None):
        super().__init__(f"Invalid file opening mode '{mode}'.", location)
        self.mode = mode