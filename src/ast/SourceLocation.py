from dataclasses import dataclass

@dataclass(frozen=True)
class SourceLocation:
    filename: str
    line: int
    column: int

    def __str__(self):
        return f"{self.filename}:{self.line}:{self.column}"