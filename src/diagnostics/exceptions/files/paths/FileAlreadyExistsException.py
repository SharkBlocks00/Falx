from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOError

class FileAlreadyExistsException(IOError):
    def __init__(self, path: str, location: SourceLocation | None = None):
        super().__init__(f"File '{path}' already exists.", location)
        self.path = path