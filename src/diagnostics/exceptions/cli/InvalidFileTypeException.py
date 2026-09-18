from src.diagnostics.exceptions.cli.FalxCLIException import FalxCLIException


class InvalidFileTypeException(FalxCLIException):
    def __init__(self, flag: str):
        super().__init__(f"Invalid filetype after '{flag}'")
