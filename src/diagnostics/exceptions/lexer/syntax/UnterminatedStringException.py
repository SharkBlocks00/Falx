from src.ast.SourceLocation import SourceLocation
from src.diagnostics.exceptions.lexer.FalxLexerException import FalxLexerException


class UnterminatedStringException(FalxLexerException):
    def __init__(self, location: SourceLocation | None = None):
        super().__init__("Unterminated string.", location)