from src.diagnostics.exceptions.cli.FalxCLIException import FalxCLIException


class MissingFilenameException(FalxCLIException):
    def __init__(self, flag: str):
        super().__init__(f"Missing filename after '{flag}'")
