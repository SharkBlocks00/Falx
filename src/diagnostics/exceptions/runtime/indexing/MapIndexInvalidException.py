from src.diagnostics.exceptions.runtime.indexing.IndexInvalidException import IndexInvalidException


class MapIndexInvalidException(IndexInvalidException):
    def __init__(self, index: int | str, size: int) -> None:
        super().__init__(index, size, "map")

    @staticmethod
    def check(index: int, size: int) -> None:
        if index >= size:
            raise MapIndexInvalidException(index, size)