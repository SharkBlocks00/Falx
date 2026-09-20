from __future__ import annotations

from pathlib import Path

from src.diagnostics.exceptions.runtime.modules.CircularModuleDependencyException import \
    CircularModuleDependencyException
from src.diagnostics.exceptions.runtime.modules.ModuleNotFoundException import ModuleNotFoundException
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
        self.loadingModules: list[Path] = []

    def load(self, name: str) -> FalxModule:
        from src.parser.Parser import Parser
        path = self._resolve(name)

        if path in self.loadingModules:
            raise CircularModuleDependencyException(path, self.loadingModules)

        self.loadingModules.append(path)

        try:

            source = path.read_text(encoding="utf-8")

            tokens = Lexer(source, filename=path.name).lex()
            statements = Parser(tokens).parse()

            environment = Environment(self.interpreter.globals)

            self.interpreter.execute(statements, environment)

            return FalxModule(name, environment)
        finally:
            self.loadingModules.pop()

    def _resolve(self, name: str) -> Path:
        path = self.project_directory / f"{name}.flx"

        if not path.is_file():
            raise ModuleNotFoundException(name, path.__str__())

        return path