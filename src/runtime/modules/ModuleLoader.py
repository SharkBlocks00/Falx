from __future__ import annotations

from pathlib import Path

from src.lexer.Lexer import Lexer
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.runtime.Environment import Environment
from src.runtime.objects.FalxModule import FalxModule


class ModuleLoader:
    def __init__(self, interpreter: Interpreter, project_directory: Path):
        self.interpreter = interpreter
        self.project_directory = project_directory

    def load(self, name: str) -> FalxModule:
        from src.parser.Parser import Parser
        path = self._resolve(name)

        source = path.read_text(encoding="utf-8")

        tokens = Lexer(source).lex()
        statements = Parser(tokens).parse()

        environment = Environment(self.interpreter.globals)

        self.interpreter.execute(statements, environment)

        return FalxModule(name, environment)

    def _resolve(self, name: str) -> Path:
        path = self.project_directory / f"{name}.flx"

        if not path.is_file():
            raise RuntimeError(f"Module '{name}' not found")

        return path