from src.modules.internal.fio.method.CloseMethod import CloseMethod
from src.modules.internal.fio.method.OpenMethod import OpenMethod
from src.modules.internal.fio.method.ReadMethod import ReadMethod
from src.modules.internal.fio.method.WriteMethod import WriteMethod
from src.runtime.Environment import Environment
from src.runtime.objects.FalxNull import FalxNull

FIO_ENVIRONMENT: Environment = Environment()

FIO_ENVIRONMENT.define("open", OpenMethod(FalxNull()), False)
FIO_ENVIRONMENT.define("close", CloseMethod(FalxNull()), False)
FIO_ENVIRONMENT.define("read", ReadMethod(FalxNull()), False)
FIO_ENVIRONMENT.define("write", WriteMethod(FalxNull()), False)