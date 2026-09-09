from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.runtime.Environment import Environment
from src.runtime.Interpreter import Interpreter
from src.runtime.values.FieldDefinition import FieldDefinition
from src.runtime.values.StructConstructor import StructConstructor
from src.runtime.values.StructDefinition import StructDefinition


class StructDeclaration(Statement):
    def __init__(self, location: SourceLocation, name: str, fields: list[FieldDefinition]):
        super().__init__(location)
        self.name = name
        self.fields = fields

    def execute(self, interpreter: Interpreter, environment: Environment) -> None:
        definition: StructDefinition = StructDefinition(self.name, self.fields)
        constructor: StructConstructor = StructConstructor(definition)

        environment.define(self.name, constructor, False)