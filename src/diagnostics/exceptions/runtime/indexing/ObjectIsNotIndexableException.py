from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class ObjectIsNotIndexableException(FalxRuntimeException):
    def __init__(self, obj: str):
        super().__init__(f"'{obj}' is not indexable")