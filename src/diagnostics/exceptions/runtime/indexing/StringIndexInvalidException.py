from src.diagnostics.exceptions.runtime.indexing.IndexInvalidException import IndexInvalidException


class StringIndexInvalidException(IndexInvalidException):
    def __init__(self, index: int, size: int) -> None:
        super().__init__(index, size, "string")

    @staticmethod
    def check(index: int, size: int) -> None:
        if index < 0 or index >= size:
            raise StringIndexInvalidException(index, size)