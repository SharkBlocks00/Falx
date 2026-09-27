from __future__ import annotations

from pathlib import Path
from typing import TYPE_CHECKING

from src.diagnostics.exceptions.files.IOErrorException import IOErrorException
from src.diagnostics.exceptions.files.NotADirectoryException import NotADirectoryException
from src.diagnostics.exceptions.files.NotAFileException import NotAFileException
from src.diagnostics.exceptions.files.PermissionDeniedException import PermissionDeniedException
from src.diagnostics.exceptions.files.modes.InvalidFileModeException import InvalidFileModeException
from src.diagnostics.exceptions.files.paths.FileAlreadyExistsException import FileAlreadyExistsException
from src.diagnostics.exceptions.files.paths.FileNotFoundException import FileNotFoundException
from src.runtime.methods.common.BoundNativeFunction import BoundNativeFunction
from src.runtime.objects.FalxFile import FalxFile
from src.runtime.objects.FalxValue import FalxValue

if TYPE_CHECKING:
    from src.runtime.Interpreter import Interpreter

class OpenMethod(BoundNativeFunction):
    def __init__(self, this: FalxValue) -> None:
        super().__init__(this)

    def arity(self) -> int:
        return 2

    def isStrict(self) -> bool:
        return True

    def call(self, interpreter: Interpreter, arguments: list[FalxValue]) -> FalxValue:
        path = Path(arguments[0].asString())
        mode = arguments[1].asString()

        if "b" in mode:
            raise InvalidFileModeException(mode)

        if not path.is_absolute() and interpreter.currentFile is not None:
            path = interpreter.currentFile.parent / path

        try:
            path = path.resolve()
            file = open(path, mode)
        except FileNotFoundError:
            raise FileNotFoundException(str(path))
        except PermissionError:
            raise PermissionDeniedException(str(path))
        except FileExistsError:
            raise FileAlreadyExistsException(str(path))
        except IsADirectoryError:
            raise NotAFileException(str(path))
        except NotADirectoryError:
            raise NotADirectoryException(str(path))
        except ValueError:
            raise InvalidFileModeException(mode)
        except OSError as e:
            raise IOErrorException(str(e)) from e

        return FalxFile(file)



    def __eq__(self, other: object) -> bool:
        return isinstance(other, OpenMethod) and self.this == other.this

    def __hash__(self) -> int:
        return hash(self.this)