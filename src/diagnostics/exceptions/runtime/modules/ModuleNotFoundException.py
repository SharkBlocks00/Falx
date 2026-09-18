from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class ModuleNotFoundException(FalxRuntimeException):
    def __init__(self, name: str, path: str):
        super().__init__(f"Cannot find module '{name}' in path '{path}'")