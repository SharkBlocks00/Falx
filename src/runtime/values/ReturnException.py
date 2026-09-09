from src.runtime.objects.FalxValue import FalxValue


class ReturnException(Exception):
    def __init__(self, value: FalxValue) -> None:
        super().__init__()
        self.value = value

