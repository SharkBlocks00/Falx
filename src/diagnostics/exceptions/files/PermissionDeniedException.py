from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.files.IOErrorException import IOError

class PermissionDeniedException(IOError):
    def __init__(self, path: str, location: SourceLocation | None = None):
        super().__init__(f"You do not have permission to work on '{path}'.", location)
        self.path = path