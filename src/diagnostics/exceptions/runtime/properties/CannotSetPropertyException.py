from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotSetPropertyException(FalxRuntimeException):
    def __init__(self, obj: str, propertyName: str):
        super().__init__(f"'{obj}' has no settable property '{propertyName}'")