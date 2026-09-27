from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOErrorException

class FileNotFoundException(IOErrorException):
    def __init__(self, path: str, location: SourceLocation | None = None):
        super().__init__(f"File '{path}' does not exist.", location)
        self.path = path