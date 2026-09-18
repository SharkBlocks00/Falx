from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.parser.FalxParserException import FalxParserException


class ReturnOutsideFunctionException(FalxParserException):
    def __init__(self, location: SourceLocation | None = None):
        super().__init__("'return' called outside a valid function", location)