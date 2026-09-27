from src.modules.internal.random.methods.GetStateMethod import GetStateMethod
from src.modules.internal.random.methods.RandomIntMethod import RandomIntMethod
from src.modules.internal.random.methods.RandomMethod import RandomMethod
from src.modules.internal.random.methods.SeedMethod import SeedMethod
from src.modules.internal.random.methods.SetStateMethod import SetStateMethod
from src.runtime.Environment import Environment
from src.runtime.objects.FalxNull import FalxNull

RANDOM_ENVIRONMENT: Environment = Environment()

# Functions

RANDOM_ENVIRONMENT.define("random", RandomMethod(FalxNull()), False)
RANDOM_ENVIRONMENT.define("random_int", RandomIntMethod(FalxNull()), False)
RANDOM_ENVIRONMENT.define("seed", SeedMethod(FalxNull()), False)
RANDOM_ENVIRONMENT.define("get_state", GetStateMethod(FalxNull()), False)
RANDOM_ENVIRONMENT.define("set_state", SetStateMethod(FalxNull()), False)