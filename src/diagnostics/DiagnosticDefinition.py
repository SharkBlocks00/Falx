from dataclasses import dataclass

from src.diagnostics.DiagnosticCode import DiagnosticCode
from src.diagnostics.DiagnosticSeverity import DiagnosticSeverity


@dataclass(frozen=True)
class DiagnosticDefinition:
    """The static definition of a diagnostic"""
    code: DiagnosticCode
    severity: DiagnosticSeverity
    title: str
    help: tuple[str, ...] = ()