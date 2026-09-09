from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from abc import ABC, abstractmethod

from src.runtime.objects.FalxValue import FalxValue

class Callable(FalxValue, ABC):
    @abstractmethod
    def arity(self) -> int:
        pass

    @abstractmethod
    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        pass