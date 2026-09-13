import sys

from src.diagnostics.Diagnostic import Diagnostic


class DiagnosticPrinter:
    def print(self, diagnostic: Diagnostic, source: str) -> None:
        if diagnostic.location is None:
            print(f"{diagnostic.severity.value}[{diagnostic.code.value}]: {diagnostic.message}", file=sys.stderr)
            return

        location = diagnostic.location
        lines = source.splitlines()

        lineNumberWidth = len(str(len(lines)))

        print(
            f"{location.filename}:"
            f"{location.line}:"
            f"{location.column}: "
            f"{diagnostic.severity.value}"
            f"[{diagnostic.code.value}]: "
            f"{diagnostic.message}",
            file=sys.stderr
        )

        if location.line <= len(lines):
            errorLine = lines[location.line - 1]

            print(f"  {location.line:>{lineNumberWidth}} | {errorLine}", file=sys.stderr)

            print(f"  {' ' * lineNumberWidth} | {' ' * (location.column -1)}^", file=sys.stderr)

        for helpMessage in diagnostic.help:
            print(f"help: {helpMessage}", file=sys.stderr)
        print()
        for note in diagnostic.notes:
            print(f"note: {note}", file=sys.stderr)