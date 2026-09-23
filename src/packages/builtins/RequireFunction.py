from __future__ import annotations

from pathlib import Path
from typing import TYPE_CHECKING


if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter
    from src.runtime.modules.ModuleLoader import ModuleLoader

from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxValue import FalxValue

class RequireFunction(NativeFunction):
    """require(module) loads a local .flx file/module"""

    def __init__(self, moduleLoader: ModuleLoader, relativeTo: Path | None = None):
        super().__init__()
        self.moduleLoader: ModuleLoader = moduleLoader
        self.relativeTo: Path | None = relativeTo

    def __hash__(self) -> int:
        return hash(self.__class__)

    def __eq__(self, other) -> bool:
        return isinstance(other, RequireFunction) and self.moduleLoader is other.moduleLoader and self.relativeTo is other.relativeTo

    def isStrict(self) -> bool:
        return True

    def arity(self) -> int:
        return 1

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        moduleName = arguments[0].asString()

        return self.moduleLoader.load(moduleName, self.relativeTo)

    def __str__(self) -> str:
        return "<require function>"