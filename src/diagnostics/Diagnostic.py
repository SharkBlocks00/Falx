from dataclasses import dataclass, field

from src.ast.SourceLocation import SourceLocation
from src.diagnostics.DiagnosticCode import DiagnosticCode
from src.diagnostics.DiagnosticSeverity import DiagnosticSeverity


@dataclass
class Diagnostic:
    code: DiagnosticCode
    severity: DiagnosticSeverity
    message: str
    location: SourceLocation | None

    notes: list[str] = field(default_factory=list)
    help: list[str] = field(default_factory=list)