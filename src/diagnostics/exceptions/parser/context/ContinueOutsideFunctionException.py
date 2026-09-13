from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.parser.FalxParserException import FalxParserException


class ContinueOutsideFunctionException(FalxParserException):
    def __init__(self, location: SourceLocation | None = None):
        super().__init__("'continue' called outside a valid function", location)