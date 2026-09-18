from src.diagnostics.exceptions.runtime.indexing.IndexInvalidException import IndexInvalidException


class ArrayIndexInvalidException(IndexInvalidException):
    def __init__(self, index: int, size: int) -> None:
        super().__init__(index, size, "array")

    @staticmethod
    def check(index: int, size: int) -> None:
        if index >= size:
            raise ArrayIndexInvalidException(index, size)