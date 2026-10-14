from src.runtime.objects.FalxValue import FalxValue


class ReturnException(Exception):
    """Exception raised in order to know when a `return` statement is called."""

    def __init__(self, value: FalxValue) -> None:
        super().__init__()
        self.value = value

