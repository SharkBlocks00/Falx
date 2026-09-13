from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.parser.FalxParserException import FalxParserException


class UnexpectedEndOfInputException(FalxParserException):
    def __init__(self, expected: str, location: SourceLocation | None = None):
        super().__init__(expected, location)