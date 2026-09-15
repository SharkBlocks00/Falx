from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class IndexInvalidException(FalxRuntimeException):
    def __init__(self, index: int, size: int, _type: str | None = None) -> None:
        super().__init__(f"Index '{index}' is invalid for {_type if _type is not None else 'structure'} with size '{size}'")

    @staticmethod
    def check(index: int, size: int) -> None:
        if index < 0 or index >= size:
            raise IndexInvalidException(index, size)