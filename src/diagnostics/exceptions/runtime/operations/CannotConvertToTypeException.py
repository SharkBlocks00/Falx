from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotConvertToTypeException(FalxRuntimeException):
    def __init__(self, name: str, expectedType: str):
        super().__init__(f"Cannot convert '{name}' to {expectedType}")
        self.expected = expectedType
        self.name = name