from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CannotEvaluateValueException(FalxRuntimeException):
    def __init__(self, leftValue: str, rightValue: str) -> None:
        super().__init__(f"Cannot evaluate '{leftValue}' and '{rightValue}'.")