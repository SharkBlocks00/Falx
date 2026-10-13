from __future__ import annotations

from src.ast.Variable import Variable
from src.diagnostics.exceptions.runtime.variables.AssignToConstantException import AssignToConstantException
from src.diagnostics.exceptions.runtime.variables.UndefinedVariableException import UndefinedVariableException
from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException
from src.runtime.objects.FalxValue import FalxValue

class Environment:
    """Environment class for Falx, nothing should inherit this,
    as many copies of this class can be made as needed
    """

    def __init__(self, parent: Environment = None, isStandardLibrary: bool = False):
        """Initialize the environment and it's components.

        Args:
            parent (Environment, optional): The parent Environment to use, if passed.
            isStandardLibrary (bool, optional): Whether this environment is
                being used as a standard library's environment.
        """

        self.parent: Environment = parent
        self.variables: dict[str, Variable] = {}

        self.isStandardLibrary: bool = isStandardLibrary or (
            parent is not None and parent.isStandardLibrary
        )

    def define(self, name: str, value: FalxValue, mutable: bool = True) -> None:
        """Define a variable in the environment.

        Args:
            name (str): The name of the variable to be defined.
            value (FalxValue): The value to be defined.
            mutable (bool, optional): Whether the variable is mutable.

        Raises:
            VariableAlreadyExistsException: If the given variable name
                already exists in the environment.
        """
        if self.variables.get(name) is not None:
            raise VariableAlreadyExistsException(name)

        self.variables[name] = Variable(value, mutable=mutable)

    def assign(self, name: str, value: FalxValue) -> None:
        """Assign a new value to a pre-defined variable.

        Args:
            name (str): The name of the variable to be assigned to.
            value (FalxValue): The value to be assigned.

        Raises:
            AssignToConstantException: If the variable with the provided
                name is a constant, or immutable variable.
            UndefinedVariableException: If the provided name cannot be
                found in the environment.
        """
        variable: Variable = self.variables.get(name)

        if variable is not None:
            if not variable.mutable:
                raise AssignToConstantException(name)
            variable.value = value
            return

        if self.parent is not None:
            self.parent.assign(name, value)
            return

        raise UndefinedVariableException(name)

    def checkAssignable(self, name: str) -> None:
        """Checks if a variable can be assigned to.

        Finds whether the variable with the provided name can be assigned to,
        without modifying the environment.

        Args:
            name (str): The name of the variable to be checked.

        Raises:
            AssignToConstantException: If the variable with the provided
                name is a constant, or immutable variable.
            UndefinedVariableException: If the provided name cannot be
                found in the environment.
        """
        variable: Variable = self.variables.get(name)

        if variable is not None:
            if not variable.mutable:
                raise AssignToConstantException(name)
            return

        if self.parent is not None:
            self.parent.checkAssignable(name)
            return

        raise UndefinedVariableException(name)

    def get(self, name: str) -> FalxValue:
        """Returns a variable's value from the environment.
        Includes the parent environment if it exists.

        Args:
            name (str): The name of the variable to be returned.

        Raises:
            UndefinedVariableException: If the provided name cannot be
                found in the environment.

        Returns:
            FalxValue: The value of the variable.
        """

        variable: Variable = self.variables.get(name)

        if variable is not None:
            return variable.value

        if self.parent is not None:
            return self.parent.get(name)

        raise UndefinedVariableException(name)

    def getLocal(self, name: str) -> FalxValue:
        """Returns a variable's value from the local environment.
        Does not include the parent environment, even if it does exist.

        Args:
            name (str): The name of the variable to be returned.

        Raises:
            UndefinedVariableException: If the provided name cannot be
                found in the local environment.

        Returns:
            FalxValue: The value of the variable.
        """
        variable: Variable = self.variables.get(name)

        if variable is not None:
            return variable.value

        raise UndefinedVariableException(name)

    def assignLocal(self, name: str, value: FalxValue) -> None:
        """Assign a new value to a local variable.

        Args:
            name (str): The name of the variable to be assigned to.
            value (FalxValue): The value to be assigned.

        Raises:
            AssignToConstantException: If the variable with the provided
                name is a constant, or immutable variable.
            UndefinedVariableException: If the provided name cannot be
                found in the environment.
        """
        variable: Variable = self.variables.get(name)

        if variable is not None:
            if not variable.mutable:
                raise AssignToConstantException(name)
            variable.value = value
            return

        raise UndefinedVariableException(name)
