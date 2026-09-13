from enum import Enum

class DiagnosticCode(Enum):
    """Enum class for all error/exception codes"""
    # Runtime errors
    UNDEFINED_VARIABLE = "E1000"
    UNDEFINED_PROPERTY = "E1001"
    INVALID_OPERATION = "E1002"
    DIVISION_BY_ZERO = "E1003"

    # Parser errors
    SYNTAX_ERROR = "E2000"
    UNEXPECTED_TOKEN = "E2001"

    # Lexer errors
    INVALID_CHARACTER = "E3000"

    # Module errors
    MODULE_NOT_FOUND = "E4000"