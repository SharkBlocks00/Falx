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
    RETURN_OUTSIDE_FUNCTION = "E2002"
    CONTINUE_OUTSIDE_LOOP = "E2003"
    BREAK_OUTSIDE_FUNCTION = "E2004"
    INVALID_ASSIGNMENT_TARGET = "E2005"

    # Lexer errors
    INVALID_CHARACTER = "E3000"
    UNTERMINATED_STRING = "E3001"
    INVALID_ESCAPE_SEQUENCE = "E3002"

    # Module errors
    MODULE_NOT_FOUND = "E4000"