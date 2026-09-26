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

    def load(self, name: str, relativeTo: Path | None = None) -> FalxModule:
        from src.parser.Parser import Parser
        from src.packages.builtins.RequireFunction import RequireFunction

        if relativeTo is None:
            relativeTo = self.loadingModules[-1] if self.loadingModules else None
        path = self._resolve(name, relativeTo)

        if path in self.loadingModules:
            raise CircularModuleDependencyException(path, self.loadingModules)

        if path in self.cachedModules:
            return self.cachedModules[path]

        moduleGlobals: Environment = Environment(self.interpreter.globals)
        moduleGlobals.define("require", RequireFunction(self, path), False)
        environment = Environment(moduleGlobals)
        module = FalxModule(name, environment)

        self.cachedModules[path] = module
        self.loadingModules.append(path)

        try:
            source = path.read_text(encoding="utf-8")

            tokens = Lexer(source, filename=path.name).lex()

            statements = Parser(tokens).parse()

            previousFile = self.interpreter.currentFile
            self.interpreter.currentFile = path

            try:
                self.interpreter.execute(statements, environment)
                return module
            finally:
                self.interpreter.currentFile = previousFile


        except Exception:
            del self.cachedModules[path]
            raise

        finally:
            self.loadingModules.pop()

    def loadInternal(self, name: str) -> FalxModule:
        match name:
            case "/math":
                from src.modules.internal.math.MathEnvironment import MATH_ENVIRONMENT
                return FalxModule("math", MATH_ENVIRONMENT)
            case "/random":
                from src.modules.internal.random.RandomEnvironment import RANDOM_ENVIRONMENT
                return FalxModule("random", RANDOM_ENVIRONMENT)
            case "/fio":
                from src.modules.internal.fio.FIOEnvironment import FIO_ENVIRONMENT
                return FalxModule("fio", FIO_ENVIRONMENT)
            case "/time":
                from src.modules.internal.time.TimeEnvironment import TIME_ENVIRONMENT
                return FalxModule("time", TIME_ENVIRONMENT)
            case _:
                raise ModuleNotFoundException(name, "internal module directory")

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