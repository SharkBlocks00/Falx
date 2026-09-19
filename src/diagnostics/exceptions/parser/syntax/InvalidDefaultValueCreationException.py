from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.parser.FalxParserException import FalxParserException


class InvalidDefaultValueCreationException(FalxParserException):
    def __init__(self, value: str, location: SourceLocation | None = None):
        super().__init__(f"Cannot have non default parameter {value} after default parameters.", location)