from src.diagnostics.DiagnosticCode import DiagnosticCode
from src.diagnostics.DiagnosticDefinition import DiagnosticDefinition
from src.diagnostics.DiagnosticSeverity import DiagnosticSeverity
from src.diagnostics.exceptions.lexer.syntax.InvalidCharacterException import InvalidCharacterException
from src.diagnostics.exceptions.lexer.syntax.InvalidEscapeSequenceException import InvalidEscapeSequenceException
from src.diagnostics.exceptions.lexer.syntax.UnterminatedStringException import UnterminatedStringException
from src.diagnostics.exceptions.parser.context.BreakOutsideFunction import BreakOutsideFunctionException
from src.diagnostics.exceptions.parser.context.ContinueOutsideFunctionException import ContinueOutsideFunctionException
from src.diagnostics.exceptions.parser.context.ReturnOutsideFunctionException import ReturnOutsideFunctionException
from src.diagnostics.exceptions.parser.syntax.InvalidAssignmentTargetException import InvalidAssignmentTargetException
from src.diagnostics.exceptions.parser.syntax.UnexpectedTokenException import UnexpectedTokenException
from src.diagnostics.exceptions.runtime.operations.DivisionByZeroException import DivisionByZeroException
from src.diagnostics.exceptions.runtime.variables.UndefinedVariableException import UndefinedVariableException

DIAGNOSTIC_REGISTRY: dict[..., DiagnosticDefinition] = {

    UndefinedVariableException: DiagnosticDefinition(
        code=DiagnosticCode.UNDEFINED_VARIABLE,
        severity=DiagnosticSeverity.ERROR,
        title="undefined variable",
        help=(
            "Check that the variable has been declared.",
            "Check that the variable name is spelled correctly."
        ),
        notes=("Variables must be declared before you can use them.",)
    ),

    DivisionByZeroException: DiagnosticDefinition(
        code=DiagnosticCode.DIVISION_BY_ZERO,
        severity=DiagnosticSeverity.ERROR,
        title="division by zero",
        help=(
            "Check the value of the divisor before performing the division.",
        ),
        notes=("You cannot divide by zero.",)
    ),

    ReturnOutsideFunctionException: DiagnosticDefinition(
        code=DiagnosticCode.RETURN_OUTSIDE_FUNCTION,
        severity=DiagnosticSeverity.ERROR,
        title="return outside function",
        help=(
            "Check you have not called return outside of a function.",
        ),
        notes=("Return can only be called inside of a function",)
    ),

    ContinueOutsideFunctionException: DiagnosticDefinition(
        code=DiagnosticCode.CONTINUE_OUTSIDE_LOOP,
        severity=DiagnosticSeverity.ERROR,
        title="continue outside loop",
        help=(
            "Check you have not called continue outside of a loop.",
        ),
        notes=("Continue can only be called inside of a loop",)
    ),

    BreakOutsideFunctionException: DiagnosticDefinition(
        code=DiagnosticCode.BREAK_OUTSIDE_FUNCTION,
        severity=DiagnosticSeverity.ERROR,
        title="break outside function",
        help=(
            "Check you have not called break outside of a function or loop.",
        ),
        notes=("Break can only be called inside of a function or loop.",)
    ),

    InvalidAssignmentTargetException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ASSIGNMENT_TARGET,
        severity=DiagnosticSeverity.ERROR,
        title="invalid assignment target",
        help=(
            "Check the type of the assignment target.",
        ),
        notes=("Certain data types cannot be assigned to.",)
    ),

    UnexpectedTokenException: DiagnosticDefinition(
        code=DiagnosticCode.UNEXPECTED_TOKEN,
        severity=DiagnosticSeverity.ERROR,
        title="unexpected token",
        help=(
            "Check that your syntax is correct and not missing anything.",
        )
    ),

    InvalidCharacterException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_CHARACTER,
        severity=DiagnosticSeverity.ERROR,
        title="invalid character",
        help=(
            "Check that your syntax is correct and not missing anything.",
        )
    ),

    UnterminatedStringException: DiagnosticDefinition(
        code=DiagnosticCode.UNTERMINATED_STRING,
        severity=DiagnosticSeverity.ERROR,
        title="unterminated string",
        help=(
            "Check that you have put a \" or ' at the end of the string.",
        ),
        notes=("You must enclose strings with \" at either ends.",)
    ),

    InvalidEscapeSequenceException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ESCAPE_SEQUENCE,
        severity=DiagnosticSeverity.ERROR,
        title="invalid escape sequence",
        help=(
            "Check that your escape sequence is valid.",
        ),
        notes=("Valid escape sequences include the following: '\\n', '\\t', '\\r', '\\f', '\\b', '\\\"', '\\\\'.",)
    )


}