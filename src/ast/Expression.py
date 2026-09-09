from abc import ABC, abstractmethod
from src.ast.SourceLocation import SourceLocation
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.objects.FalxValue import FalxValue


class Expression(ABC):
    def __init__(self, location: SourceLocation):
        self._location = location

    @property
    def location(self) -> SourceLocation:
        return self._location

    @abstractmethod
    def evaluate(self, interpreter: Interpreter, environment: Environment) -> FalxValue:
        pass