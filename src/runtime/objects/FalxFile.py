from src.runtime.objects.FalxValue import FalxValue


class FalxFile(FalxValue):
    def __init__(self, file) -> None:
        super().__init__()
        self.file = file

    def getTypeName(self) -> str:
        return "file"

    def __str__(self) -> str:
        return "<file>"

    def __eq__(self, other) -> bool:
        return isinstance(other, FalxFile) and self.file == other.file

    def __hash__(self) -> int:
        return hash(self.file)

    def __repr__(self):
        return self.__str__()