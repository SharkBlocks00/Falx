from src.diagnostics.DiagnosticCode import DiagnosticCode
from src.diagnostics.DiagnosticDefinition import DiagnosticDefinition
from src.diagnostics.DiagnosticSeverity import DiagnosticSeverity
from src.diagnostics.exceptions.DivisionByZeroException import DivisionByZeroException
from src.diagnostics.exceptions.UndefinedVariableException import UndefinedVariableException

DIAGNOSTIC_REGISTRY: dict[..., DiagnosticDefinition] = {

    UndefinedVariableException: DiagnosticDefinition(
        code=DiagnosticCode.UNDEFINED_VARIABLE,
        severity=DiagnosticSeverity.ERROR,
        title="undefined variable",
        help=(
            "Check that the variable has been declared.",
            "Check that the variable name is spelled correctly."
        )
    ),

    DivisionByZeroException: DiagnosticDefinition(
        code=DiagnosticCode.DIVISION_BY_ZERO,
        severity=DiagnosticSeverity.ERROR,
        title="division by zero",
        help=(
            "Check the value of the divisor before performing the division.",
        )
    )
}