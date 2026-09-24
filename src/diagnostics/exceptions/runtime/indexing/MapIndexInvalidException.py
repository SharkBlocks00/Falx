from src.diagnostics.exceptions.runtime.indexing.IndexInvalidException import IndexInvalidException


class MapIndexInvalidException(IndexInvalidException):
    def __init__(self, index: int | str, size: int) -> None:
        super().__init__(index, size, "map")

    @staticmethod
    def check(index: int, size: int) -> None:
        if size == 0 and index == 0:
            return
        if index > size or index < -size:
            raise MapIndexInvalidException(index, size)