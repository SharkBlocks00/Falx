from src.ast.SourceLocation import SourceLocation
from src.tokens.TokenKind import TokenKind


class Token:
    def __init__(self, tokenKind: TokenKind, lexeme: str, literal: object, location: SourceLocation):
        self.tokenKind = tokenKind
        self.lexeme = lexeme
        self.literal = literal
        self.location = location

    def __str__(self):
        return f"{self.tokenKind} {self.lexeme} {self.literal} {self.location}"