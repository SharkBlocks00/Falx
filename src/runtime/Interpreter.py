from src.ast.Statement import Statement
from src.packages.builtins.OutputFunction import OutputFunction
from src.packages.builtins.RequestFunction import RequestFunction
from src.packages.builtins.TypeOfFunction import TypeOfFunction
from src.runtime.Environment import Environment


class Interpreter:
    def __init__(self):
        self.globals: Environment = Environment()
        self.environment: Environment = self.globals

        self.registerBuiltins()

    def interpret(self, program: list[Statement]):
        # Because of how all the statements and expressions are structured,
        # the interpreter gets to be extremely simple
        for statement in program:
            statement.execute(self, self.environment)

    def registerBuiltins(self):
        """Registers the interpreter's builtin functions to the interpreter"""
        # immutable so no user/program can overwrite the builtin basic functions
        self.globals.define("output", OutputFunction(), False)
        self.globals.define("request", RequestFunction(), False)
        self.globals.define("typeof", TypeOfFunction(), False)