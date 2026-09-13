from src.runtime.Environment import Environment
from src.runtime.objects.FalxValue import FalxValue


class FalxModule(FalxValue):
    def __init__(self, name: str, environment: Environment):
        super().__init__()
        self.name = name
        self.environment = environment

    def getTypeName(self) -> str:
        return "module"

    def __str__(self) -> str:
        return f"<module '{self.name}'>"

    def __repr__(self) -> str:
        return self.__str__()

    def __eq__(self, other) -> bool:
        return isinstance(other, FalxModule) and self.name == other.name and self.environment == other.environment

    def __hash__(self) -> int:
        return hash(self.__class__)