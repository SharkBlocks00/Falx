import math

from src.modules.internal.math.methods.AbsMethod import AbsMethod
from src.modules.internal.math.methods.CeilMethod import CeilMethod
from src.modules.internal.math.methods.ClampMethod import ClampMethod
from src.modules.internal.math.methods.FloorMethod import FloorMethod
from src.modules.internal.math.methods.FracMethod import FracMethod
from src.modules.internal.math.methods.MaxMethod import MaxMethod
from src.modules.internal.math.methods.MinMethod import MinMethod
from src.modules.internal.math.methods.SignMethod import SignMethod
from src.modules.internal.math.methods.SqrtMethod import SqrtMethod
from src.modules.internal.math.methods.TruncMethod import TruncMethod
from src.runtime.Environment import Environment
from src.runtime.objects.FalxNull import FalxNull
from src.runtime.objects.FalxNumber import FalxNumber

MATH_ENVIRONMENT: Environment = Environment()

# Constants
MATH_ENVIRONMENT.define("pi", FalxNumber(3.141592653589793), False)
MATH_ENVIRONMENT.define("e", FalxNumber(math.e), False)
MATH_ENVIRONMENT.define("tau", FalxNumber(math.tau), False)
MATH_ENVIRONMENT.define("phi", FalxNumber(1.6180339887), False)

MATH_ENVIRONMENT.define("sqrt2", FalxNumber(math.sqrt(2)), False)
MATH_ENVIRONMENT.define("sqrt3", FalxNumber(math.sqrt(3)), False)

MATH_ENVIRONMENT.define("ln2", FalxNumber(math.log(2)), False)
MATH_ENVIRONMENT.define("ln10", FalxNumber(math.log(10)), False)
MATH_ENVIRONMENT.define("log2e", FalxNumber(1.44269504089), False)
MATH_ENVIRONMENT.define("log10e", FalxNumber(0.4342944819), False)

MATH_ENVIRONMENT.define("epsilon", FalxNumber(2.0 ** -52), False)


# Functions

# Basic operations
MATH_ENVIRONMENT.define("abs", AbsMethod(FalxNull()), False)
MATH_ENVIRONMENT.define("sign", SignMethod(FalxNull()), False)

MATH_ENVIRONMENT.define("min", MinMethod(FalxNull()), False)
MATH_ENVIRONMENT.define("max", MaxMethod(FalxNull()), False)
MATH_ENVIRONMENT.define("clamp", ClampMethod(FalxNull()), False)

MATH_ENVIRONMENT.define("floor", FloorMethod(FalxNull()), False)
MATH_ENVIRONMENT.define("ceil", CeilMethod(FalxNull()), False)
MATH_ENVIRONMENT.define("trunc", TruncMethod(FalxNull()), False)

MATH_ENVIRONMENT.define("frac", FracMethod(FalxNull()), False)

# Powers and roots
MATH_ENVIRONMENT.define("sqrt", SqrtMethod(FalxNull()), False)