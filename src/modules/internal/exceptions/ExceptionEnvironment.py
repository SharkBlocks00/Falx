from src.packages.builtins.ExceptionFunction import ExceptionFunction
from src.packages.builtins.FalxExceptionConstructor import FalxExceptionConstructor
from src.runtime.Environment import Environment
from src.diagnostics.DiagnosticRegistry import DIAGNOSTIC_REGISTRY
from src.runtime.objects.FalxThrowable import FalxThrowable

EXCEPTION_ENVIRONMENT: Environment = Environment()

EXCEPTION_ENVIRONMENT.define("Exception", ExceptionFunction(), False)

# Define all exceptions as we just simply dont expose the internal ones in std::exceptions
for exceptionType in DIAGNOSTIC_REGISTRY:
    if exceptionType is FalxThrowable:
        continue
    EXCEPTION_ENVIRONMENT.define(exceptionType.__name__, FalxExceptionConstructor(exceptionType), False)
