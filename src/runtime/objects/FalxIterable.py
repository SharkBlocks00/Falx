from abc import ABC, abstractmethod
from typing import Iterable

from src.runtime.objects.FalxValue import FalxValue


class FalxIterable(ABC):
    @abstractmethod
    def iterate(self) -> Iterable[FalxValue]:
        """Return an iterable over some FalxValues"""
        pass