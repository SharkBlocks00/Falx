from enum import Enum, auto

class TokenKind(Enum):
    """Base class that all Tokens use"""
    # Literals
    IDENTIFIER = auto()
    STRING = auto()
    NUMBER = auto()
    BOOLEAN = auto()

    # Keywords
    LET = auto()
    CONST = auto()
    FUNC = auto()
    DEFINE = auto()
    IF = auto()
    ELSE = auto()
    ELSEIF = auto()
    WHILE = auto()
    FOREACH = auto()
    RETURN = auto()
    BREAK = auto()
    CONTINUE = auto()
    STRUCT = auto()
    NULL = auto()

    # Delimiters
    LEFT_PAREN = auto()
    RIGHT_PAREN = auto()

    LEFT_BRACE = auto()
    RIGHT_BRACE = auto()

    LEFT_BRACKET = auto()
    RIGHT_BRACKET = auto()

    COMMA = auto()
    DOT = auto()
    COLON = auto()
    SEMICOLON = auto()

    # Operators
    PLUS = auto()
    MINUS = auto()
    STAR = auto()
    SLASH = auto()
    PERCENT = auto()

    PLUS_PLUS = auto()
    MINUS_MINUS = auto()
    STAR_STAR = auto()
    SLASH_SLASH = auto()

    EQUAL = auto()
    PLUS_EQUAL = auto()
    MINUS_EQUAL = auto()
    STAR_EQUAL = auto()
    SLASH_EQUAL = auto()
    PERCENT_EQUAL = auto()

    EQUAL_EQUAL = auto()
    BANG = auto()
    BANG_EQUAL = auto()

    LESS = auto()
    LESS_EQUAL = auto()

    GREATER = auto()
    GREATER_EQUAL = auto()

    AND = auto()
    OR = auto()

    # Special
    EOF = auto()