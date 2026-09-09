from abc import ABC, abstractmethod

from src.packages.Callable import Callable


class NativeFunction(Callable, ABC):
    """Builtin function base class so that
    we can easily tell if a function is
    built in or not
    """
    def __str__(self):
        return "<native function>"