from __future__ import annotations
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.runtime.Environment import Environment


class BlockStatement(Statement):
    def __init__(self, location: SourceLocation, statements: list[Statement]):
        super().__init__(location)
        self.statements = statements

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        block: Environment = Environment(environment)
        for statement in self.statements:
            statement.execute(interpreter, block) # use an internal environment for just the {} block
    