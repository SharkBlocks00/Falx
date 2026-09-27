from src.modules.internal.time.methods.LocalNowMethod import LocalNowMethod
from src.modules.internal.time.methods.MonotonicMethod import MonotonicMethod
from src.modules.internal.time.methods.SleepMethod import SleepMethod
from src.modules.internal.time.methods.TimeMethod import TimeMethod
from src.modules.internal.time.methods.UtcNowMethod import UtcNowMethod
from src.runtime.Environment import Environment
from src.runtime.objects.FalxNull import FalxNull

TIME_ENVIRONMENT: Environment = Environment()

# Methods

TIME_ENVIRONMENT.define("time", TimeMethod(FalxNull()), False)
TIME_ENVIRONMENT.define("monotonic", MonotonicMethod(FalxNull()), False)
TIME_ENVIRONMENT.define("sleep", SleepMethod(FalxNull()), False)
TIME_ENVIRONMENT.define("utc_now", UtcNowMethod(FalxNull()), False)
TIME_ENVIRONMENT.define("local_now", LocalNowMethod(FalxNull()), False)