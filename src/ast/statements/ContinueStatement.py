from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.exceptions.ContinueException import ContinueException
from src.runtime.Environment import Environment


class ContinueStatement(Statement):
    def __init__(self, location: SourceLocation):
        super().__init__(location)

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        raise ContinueException()