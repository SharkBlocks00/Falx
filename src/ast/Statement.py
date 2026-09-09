from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from abc import ABC, abstractmethod
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment



class Statement(ABC):
    def __init__(self, location: SourceLocation):
        self._location = location

    @property
    def location(self) -> SourceLocation:
        return self._location

    @abstractmethod
    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        pass