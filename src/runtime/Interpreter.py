from pathlib import Path

from src.ast.Statement import Statement
from src.diagnostics.exceptions.runtime.RecursionDepthExceededException import RecursionDepthExceededException
from src.packages.builtins.AssertFunction import AssertFunction
from src.packages.builtins.OutputFunction import OutputFunction
from src.packages.builtins.RequestFunction import RequestFunction
from src.packages.builtins.RequireFunction import RequireFunction
from src.packages.builtins.TypeOfFunction import TypeOfFunction
from src.runtime.Environment import Environment
from src.runtime.modules.ModuleLoader import ModuleLoader


class Interpreter:
    def __init__(self, projectDir: Path = Path.cwd()):
        self.globals: Environment = Environment()
        self.environment: Environment = self.globals

        self.moduleLoader = ModuleLoader(self, projectDir)

        self.registerBuiltins()

        self.maxRecursionDepth = 20

    def interpret(self, program: list[Statement]):
        # Because of how all the statements and expressions are structured,
        # the interpreter gets to be extremely simple
        self.execute(program, self.environment)

    def execute(self, program: list[Statement], environment: Environment):
        # doing this means that we can support interpreting with seperate environments,
        # useful for doing modulation
        for statement in program:
            try:
                statement.execute(self, environment)
            except RecursionError:
                raise RecursionDepthExceededException(self.maxRecursionDepth, statement.location)


    def registerBuiltins(self):
        """Registers the interpreter's builtin functions to the interpreter"""
        # immutable so no user/program can overwrite the builtin basic functions
        self.globals.define("output", OutputFunction(), False)
        self.globals.define("request", RequestFunction(), False)
        self.globals.define("typeof", TypeOfFunction(), False)
        self.globals.define("assert", AssertFunction(), False)
        self.globals.define("require", RequireFunction(self.moduleLoader), False)