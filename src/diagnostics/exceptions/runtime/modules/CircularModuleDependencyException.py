from pathlib import Path

from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException


class CircularModuleDependencyException(FalxRuntimeException):
    def __init__(self, path: Path, modules: list[Path]) -> None:
        moduleNames = [module.stem for module in modules]
        moduleNames.append(path.stem)

        dependencyChain = " -> ".join(moduleNames)

        super().__init__(f"Circular module dependency detected: {dependencyChain}")