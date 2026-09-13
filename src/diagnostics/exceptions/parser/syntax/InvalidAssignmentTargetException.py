from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.parser.FalxParserException import FalxParserException


class InvalidAssignmentTargetException(FalxParserException):
    def __init__(self, target: str, location: SourceLocation | None = None):
        super().__init__(target, location)