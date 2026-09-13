from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.parser.FalxParserException import FalxParserException


class BreakOutsideFunctionException(FalxParserException):
    def __init__(self, location: SourceLocation | None = None):
        super().__init__("'break' called outside a valid function", location)