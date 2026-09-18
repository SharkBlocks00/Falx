from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.parser.FalxParserException import FalxParserException


class UnexpectedTokenException(FalxParserException):
    def __init__(self, message: str, location: SourceLocation | None = None):
        super().__init__(message, location)