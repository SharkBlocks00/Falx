from src.modules.internal.time.methods.MonotonicMethod import MonotonicMethod
from src.modules.internal.time.methods.SleepMethod import SleepMethod
from src.modules.internal.time.methods.TimeMethod import TimeMethod
from src.runtime.Environment import Environment
from src.runtime.objects.FalxNull import FalxNull

TIME_ENVIRONMENT: Environment = Environment()

# Methods

TIME_ENVIRONMENT.define("time", TimeMethod(FalxNull()), False)
TIME_ENVIRONMENT.define("monotonic", MonotonicMethod(FalxNull()), False)
TIME_ENVIRONMENT.define("sleep", SleepMethod(FalxNull()), False)