from __future__ import annotations

from pathlib import Path
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.runtime.modules.CircularModuleDependencyException import CircularModuleDependencyException
from src.diagnostics.exceptions.runtime.modules.ModuleNotFoundException import ModuleNotFoundException
from src.lexer.Lexer import Lexer
from src.runtime.Environment import Environment
from src.runtime.objects.FalxModule import FalxModule

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter


class ModuleLoader:
    def __init__(self, interpreter: Interpreter, project_directory: Path):
        self.interpreter = interpreter
        self.project_directory = project_directory.resolve()
        self.loadingModules: list[Path] = []
        self.cachedModules: dict[Path, FalxModule] = {}

    def load(self, name: str) -> FalxModule:
        from src.parser.Parser import Parser

        relative_to = self.loadingModules[-1] if self.loadingModules else None
        path = self._resolve(name, relative_to)

        if path in self.loadingModules:
            raise CircularModuleDependencyException(path, self.loadingModules)

        if path in self.cachedModules:
            return self.cachedModules[path]

        environment = Environment(self.interpreter.globals)
        module = FalxModule(name, environment)

        self.cachedModules[path] = module
        self.loadingModules.append(path)

        try:
            source = path.read_text(encoding="utf-8")

            tokens = Lexer(source, filename=path.name).lex()

            statements = Parser(tokens).parse()

            self.interpreter.execute(statements, environment)

            return module

        except Exception:
            del self.cachedModules[path]
            raise

        finally:
            self.loadingModules.pop()

    def _resolve(self, name: str, relative_to: Path | None = None) -> Path:
        if relative_to is None:
            base_directory = self.project_directory
        else:
            base_directory = relative_to.parent

        path = (base_directory / f"{name}.flx").resolve()

        try:
            path.relative_to(self.project_directory)
        except ValueError:
            raise ModuleNotFoundException(name, path.__str__())

        if not path.is_file():
            raise ModuleNotFoundException(name, path.__str__())

        return path