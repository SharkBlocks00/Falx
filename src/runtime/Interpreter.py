import sys
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
    """Interpreter class for Falx.
       Interprets Falx programs by executing their AST statements.
    """
    def __init__(self, projectDir: Path = Path.cwd()):
        """Initialize the interpreter and its runtime environment.

        Args:
            projectDir (Path): Root directory used for resolving project modules.
        """
        self.globals: Environment = Environment()
        self.environment: Environment = self.globals

        self.moduleLoader = ModuleLoader(self, projectDir)

        self.maxRecursionDepth = 500
        self.recursionDepth = 0

        self.registerBuiltins()

        self.currentFile: Path | None = None

        sys.setrecursionlimit(5000)

    def interpret(self, program: list[Statement], file: Path | None = None) -> None:
        """Interpret a Falx program.

        Execute the program's statements in the current working environment.
        Temporarily sets the current source file for runtime context, and
        restore its previous value after execution.

        Args:
            program (list[Statement]): The statements composing the program's AST.
            file (Path | None): The source file being interpreted, if applicable.
        """

        previousFile = self.currentFile
        self.currentFile = file
        # Because of how all the statements and expressions are structured,
        # the interpreter gets to be extremely simple
        try:
            self.execute(program, self.environment)
        finally:
            self.currentFile = previousFile

    def execute(self, program: list[Statement], environment: Environment, EXPORT_ENVIRONMENT: Environment | None = None) -> None:
        """Execute a sequence of statements in the given working environment.

        Each statement is executed using the supplied environment.
        Declarations that support exports also recieve the optional
        export environment.

        Args:
            program (list[Statement]): The statements to be executed.
            environment (Environment): The environment to execute the statements in.
            EXPORT_ENVIRONMENT (Environment | None): The environment into which supported
                declarations can export their bindings, if provided.

        Raises:
            RecursionDepthExceededException: If the execution exceeds the allowed recursion depth,
                includes the raising statement's source location.

        """

        from src.ast.statements.DestructureDeclaration import DestructureDeclaration
        from src.ast.statements.FunctionDeclaration import FunctionDeclaration
        from src.ast.statements.StructDeclaration import StructDeclaration
        from src.ast.statements.VariableDeclaration import VariableDeclaration

        # doing this means that we can support interpreting with seperate environments,
        # useful for doing modulation
        for statement in program:
            try:
                if isinstance(statement, (DestructureDeclaration, FunctionDeclaration, StructDeclaration, VariableDeclaration)):
                    statement.execute(self, environment, EXPORT_ENVIRONMENT)
                else:
                    statement.execute(self, environment)
            except RecursionDepthExceededException as e:
                raise e.withLocation(statement.location)


    def registerBuiltins(self) -> None:
        """Registers the interpreter's builtin functions to the global environment."""
        # immutable so no user/program can overwrite the builtin basic functions
        self.globals.define("output", OutputFunction(), False)
        self.globals.define("request", RequestFunction(), False)
        self.globals.define("typeof", TypeOfFunction(), False)
        self.globals.define("assert", AssertFunction(), False)
        self.globals.define("require", RequireFunction(self.moduleLoader), False)

