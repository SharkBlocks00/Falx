from src.diagnostics.Diagnostic import Diagnostic
from src.diagnostics.DiagnosticRegistry import DIAGNOSTIC_REGISTRY
from src.diagnostics.exceptions.FalxException import FalxException


def createDiagnostic(error: FalxException) -> Diagnostic:
    definition = DIAGNOSTIC_REGISTRY.get(type(error))

    if definition is None:
        raise RuntimeError(f"No diagnostic definition registered for '{type(error).__name__}'")

    return Diagnostic(definition.code, definition.severity, error.message, error.location, list(definition.notes), list(definition.help))