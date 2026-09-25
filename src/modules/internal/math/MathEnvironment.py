import math

from src.runtime.Environment import Environment
from src.runtime.objects.FalxNumber import FalxNumber

MATH_ENVIRONMENT: Environment = Environment()

MATH_ENVIRONMENT.define("pi", FalxNumber(math.pi), False)

