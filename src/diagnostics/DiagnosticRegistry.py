from src.diagnostics.DiagnosticCode import DiagnosticCode
from src.diagnostics.DiagnosticDefinition import DiagnosticDefinition
from src.diagnostics.DiagnosticSeverity import DiagnosticSeverity
from src.diagnostics.exceptions.cli.InvalidFileTypeException import InvalidFileTypeException
from src.diagnostics.exceptions.cli.MissingFilenameException import MissingFilenameException
from src.diagnostics.exceptions.lexer.syntax.InvalidCharacterException import InvalidCharacterException
from src.diagnostics.exceptions.lexer.syntax.InvalidEscapeSequenceException import InvalidEscapeSequenceException
from src.diagnostics.exceptions.lexer.syntax.UnterminatedStringException import UnterminatedStringException
from src.diagnostics.exceptions.parser.context.BreakOutsideLoopException import BreakOutsideLoopException
from src.diagnostics.exceptions.parser.context.ContinueOutsideLoopException import ContinueOutsideLoopException
from src.diagnostics.exceptions.parser.context.ReturnOutsideFunctionException import ReturnOutsideFunctionException
from src.diagnostics.exceptions.parser.syntax.InvalidAssignmentTargetException import InvalidAssignmentTargetException
from src.diagnostics.exceptions.parser.syntax.InvalidDefaultValueCreationException import \
    InvalidDefaultValueCreationException
from src.diagnostics.exceptions.parser.syntax.UnexpectedEndOfInputException import UnexpectedEndOfInputException
from src.diagnostics.exceptions.parser.syntax.UnexpectedTokenException import UnexpectedTokenException
from src.diagnostics.exceptions.runtime.FalxRuntimeException import FalxRuntimeException
from src.diagnostics.exceptions.runtime.ObjectNotIterableException import ObjectNotIterableException
from src.diagnostics.exceptions.runtime.RecursionDepthExceededException import RecursionDepthExceededException
from src.diagnostics.exceptions.runtime.indexing.ArrayIndexInvalidException import ArrayIndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.IndexInvalidException import IndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.InvalidIndexTypeException import InvalidIndexTypeException
from src.diagnostics.exceptions.runtime.indexing.StringIndexInvalidException import StringIndexInvalidException
from src.diagnostics.exceptions.runtime.methods.DuplicateParameterException import DuplicateParameterException
from src.diagnostics.exceptions.runtime.methods.InvalidArgumentCountException import InvalidArgumentCountException
from src.diagnostics.exceptions.runtime.methods.ObjectNotCallableException import ObjectNotCallableException
from src.diagnostics.exceptions.runtime.modules.CircularModuleDependencyException import \
    CircularModuleDependencyException
from src.diagnostics.exceptions.runtime.modules.ModuleNotFoundException import ModuleNotFoundException
from src.diagnostics.exceptions.runtime.operations.CannotAddWithValueException import CannotAddWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotCompareToException import CannotCompareToException
from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.diagnostics.exceptions.runtime.operations.CannotDivideByValueException import CannotDivideByValueException
from src.diagnostics.exceptions.runtime.operations.CannotEvaluateValueException import CannotEvaluateValueException
from src.diagnostics.exceptions.runtime.operations.CannotMinusFromValueException import CannotMinusFromValueException
from src.diagnostics.exceptions.runtime.operations.CannotModWithValueException import CannotModWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotMultiplyByValueException import CannotMultiplyByValueException
from src.diagnostics.exceptions.runtime.operations.DivisionByZeroException import DivisionByZeroException
from src.diagnostics.exceptions.runtime.properties.CannotAccessPropertyException import CannotAccessPropertyException
from src.diagnostics.exceptions.runtime.properties.CannotSetPropertyException import CannotSetPropertyException
from src.diagnostics.exceptions.runtime.indexing.ObjectIsNotIndexableException import ObjectIsNotIndexableException
from src.diagnostics.exceptions.runtime.typing.ImmutableValueException import ImmutableValueException
from src.diagnostics.exceptions.runtime.typing.UnexpectedTypeException import UnexpectedTypeException
from src.diagnostics.exceptions.runtime.variables.AssignToConstantException import AssignToConstantException
from src.diagnostics.exceptions.runtime.variables.DestructureCountException import DestructureCountException
from src.diagnostics.exceptions.runtime.variables.UndefinedVariableException import UndefinedVariableException
from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException
from src.diagnostics.exceptions.runtime.AssertionFailedException import AssertionFailedException

DIAGNOSTIC_REGISTRY: dict[..., DiagnosticDefinition] = {

    UndefinedVariableException: DiagnosticDefinition(
        code=DiagnosticCode.UNDEFINED_VARIABLE,
        severity=DiagnosticSeverity.ERROR,
        title="undefined variable",
        help=(
            "Check that the variable has been declared before it is used.",
            "Check that the variable name is spelled correctly.",
        ),
        notes=(
            "Variables must be declared using `let` or `const` before they can be used.",
        )
    ),

    DivisionByZeroException: DiagnosticDefinition(
        code=DiagnosticCode.DIVISION_BY_ZERO,
        severity=DiagnosticSeverity.ERROR,
        title="division by zero",
        help=(
            "Check the value being used as the divisor.",
            "Make sure the divisor cannot evaluate to zero before performing the division.",
        ),
        notes=(
            "Division by zero is not a valid operation in Falx.",
        )
    ),

    ReturnOutsideFunctionException: DiagnosticDefinition(
        code=DiagnosticCode.RETURN_OUTSIDE_FUNCTION,
        severity=DiagnosticSeverity.ERROR,
        title="return outside function",
        help=(
            "Move the `return` statement inside a function declaration.",
        ),
        notes=(
            "The `return` statement can only be used while executing a function.",
        )
    ),

    ContinueOutsideLoopException: DiagnosticDefinition(
        code=DiagnosticCode.CONTINUE_OUTSIDE_LOOP,
        severity=DiagnosticSeverity.ERROR,
        title="continue outside loop",
        help=(
            "Move the `continue` statement inside a loop.",
        ),
        notes=(
            "The `continue` statement can only be used inside a loop.",
        )
    ),

    BreakOutsideLoopException: DiagnosticDefinition(
        code=DiagnosticCode.BREAK_OUTSIDE_LOOP,
        severity=DiagnosticSeverity.ERROR,
        title="break outside loop",
        help=(
            "Move the `break` statement inside a loop.",
        ),
        notes=(
            "The `break` statement can only be used inside a loop.",
        )
    ),

    InvalidAssignmentTargetException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ASSIGNMENT_TARGET,
        severity=DiagnosticSeverity.ERROR,
        title="invalid assignment target",
        help=(
            "Check that the left-hand side of the assignment is a valid assignment target.",
        ),
        notes=(
            "Only assignable variables, properties, and other supported targets can appear on the left-hand side of an assignment.",
        )
    ),

    UnexpectedTokenException: DiagnosticDefinition(
        code=DiagnosticCode.UNEXPECTED_TOKEN,
        severity=DiagnosticSeverity.ERROR,
        title="unexpected token",
        help=(
            "Check the surrounding syntax for a missing, extra, or incorrectly placed token.",
            "Check that the statement follows the expected Falx syntax.",
        ),
    ),

    InvalidCharacterException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_CHARACTER,
        severity=DiagnosticSeverity.ERROR,
        title="invalid character",
        help=(
            "Remove the invalid character or replace it with a valid Falx character.",
            "Check that the character is not the result of a typing or encoding error.",
        ),
        notes=(
            "The lexer encountered a character that is not valid in the current context.",
        )
    ),

    UnterminatedStringException: DiagnosticDefinition(
        code=DiagnosticCode.UNTERMINATED_STRING,
        severity=DiagnosticSeverity.ERROR,
        title="unterminated string",
        help=(
            "Add the missing closing quote to the string.",
            "Check that an escaped quote has not been unintentionally used or omitted.",
        ),
        notes=(
            "String literals must begin and end with matching quotation marks.",
        )
    ),

    InvalidEscapeSequenceException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ESCAPE_SEQUENCE,
        severity=DiagnosticSeverity.ERROR,
        title="invalid escape sequence",
        help=(
            "Replace the invalid escape sequence with a supported escape sequence.",
        ),
        notes=(
            "Supported escape sequences include `\\n`, `\\t`, `\\r`, `\\f`, `\\b`, `\\\"`, and `\\\\`.",
        )
    ),

    AssignToConstantException: DiagnosticDefinition(
        code=DiagnosticCode.ASSIGN_TO_CONSTANT,
        severity=DiagnosticSeverity.ERROR,
        title="assign to constant",
        help=(
            "Remove the assignment or declare the variable using `let` if its value needs to change.",
        ),
        notes=(
            "Variables declared using `const` cannot be assigned a new value after declaration.",
        )
    ),

    VariableAlreadyExistsException: DiagnosticDefinition(
        code=DiagnosticCode.VARIABLE_ALREADY_EXISTS,
        severity=DiagnosticSeverity.ERROR,
        title="variable already exists",
        help=(
            "Choose a different variable name or assign a new value to the existing variable.",
            "If you are assigning to an existing variable, remove `let` or `const` from the declaration.",
        ),
        notes=(
            "A variable cannot be declared more than once in the same environment.",
        )
    ),

    CannotMultiplyByValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_MULTIPLY_BY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot multiply by value",
        help=(
            "Check that both operands support multiplication with each other.",
        ),
        notes=(
            "The `*` operator only supports combinations of data types that define multiplication.",
        )
    ),

    CannotDivideByValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_DIVIDE_BY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot divide by value",
        help=(
            "Check that both operands support division with each other.",
            "If the divisor is valid but evaluates to zero, check for a division-by-zero error instead.",
        ),
        notes=(
            "The `/` operator only supports combinations of data types that define division.",
        )
    ),

    CannotAddWithValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_ADD_WITH_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot add with value",
        help=(
            "Check that both operands support addition with each other.",
        ),
        notes=(
            "The `+` operator only supports combinations of data types that define addition.",
        )
    ),

    CannotModWithValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_MOD_WITH_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot mod with value",
        help=(
            "Check that both operands support the modulus operation.",
        ),
        notes=(
            "The `%` operator only supports combinations of data types that define modulus.",
        )
    ),

    CannotCompareToException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_COMPARE_TO,
        severity=DiagnosticSeverity.ERROR,
        title="cannot compare to",
        help=(
            "Check that the values being compared support the selected comparison operator.",
        ),
        notes=(
            "Comparison operators can only be used with values that support comparison.",
        )
    ),

    CannotConvertToTypeException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_CONVERT_TO_TYPE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot convert to type",
        help=(
            "Check that the value can be converted to the requested type.",
            "Use a type conversion that is supported by the value and target type.",
        ),
        notes=(
            "Not every data type can be converted to every other data type.",
            "Type conversion is only supported where Falx defines a valid conversion.",
        )
    ),

    CannotAccessPropertyException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_ACCESS_PROPERTY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot access property",
        help=(
            "Check that the property exists on the value being accessed.",
            "Check that the value supports property access.",
        ),
        notes=(
            "A property can only be accessed when the object's type provides that property.",
        )
    ),

    ObjectIsNotIndexableException: DiagnosticDefinition(
        code=DiagnosticCode.OBJECT_IS_NOT_INDEXABLE,
        severity=DiagnosticSeverity.ERROR,
        title="object is not indexable",
        help=(
            "Check that the value supports indexing with `[]`.",
        ),
        notes=(
            "Only indexable values can be accessed using an index.",
        )
    ),

    CannotSetPropertyException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_SET_PROPERTY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot set property",
        help=(
            "Check that the property exists and can be modified.",
            "Check that the value allows the property to be changed.",
        ),
        notes=(
            "A property cannot be assigned when it does not exist or is not modifiable.",
        )
    ),

    CannotEvaluateValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_EVALUATE_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot evaluate value",
        help=(
            "Check that the operator is valid for the values being evaluated.",
            "Check that the expression uses a supported operator.",
        ),
        notes=(
            "Falx supports arithmetic operators such as `+`, `-`, `*`, `/`, and `%`.",
            "Falx also supports comparison operators such as `>`, `>=`, `<`, `<=`, `==`, and `!=`.",
        )
    ),

    IndexInvalidException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_INDEX,
        severity=DiagnosticSeverity.ERROR,
        title="invalid index",
        help=(
            "Check that the index is valid for the value being accessed.",
        ),
        notes=(
            "An index must be valid for the type and size of the value being indexed.",
        )
    ),

    ArrayIndexInvalidException: DiagnosticDefinition(
        code=DiagnosticCode.ARRAY_INVALID_INDEX,
        severity=DiagnosticSeverity.ERROR,
        title="invalid index on array",
        help=(
            "Check that the index is within the valid range of the array.",
        ),
        notes=(
            "An array index must refer to an element that exists within the array.",
        )
    ),

    StringIndexInvalidException: DiagnosticDefinition(
        code=DiagnosticCode.STRING_INVALID_INDEX,
        severity=DiagnosticSeverity.ERROR,
        title="invalid index on string",
        help=(
            "Check that the index is within the valid range of the string.",
        ),
        notes=(
            "A string index must refer to a character that exists within the string.",
        )
    ),

    CannotMinusFromValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_MINUS_FROM_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot subtract from value",
        help=(
            "Check that the operands support subtraction with each other.",
        ),
        notes=(
            "The `-` operator only supports combinations of data types that define subtraction.",
        )
    ),

    ObjectNotCallableException: DiagnosticDefinition(
        code=DiagnosticCode.OBJECT_NOT_CALLABLE,
        severity=DiagnosticSeverity.ERROR,
        title="object not callable",
        help=(
            "Check that the value you are trying to call is a function or another callable value.",
        ),
        notes=(
            "Only callable values can be followed by a function call.",
        )
    ),

    InvalidArgumentCountException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ARGUMENT_COUNT,
        severity=DiagnosticSeverity.ERROR,
        title="invalid argument count",
        help=(
            "Check the function declaration and provide the expected number of arguments.",
        ),
        notes=(
            "A function call must provide the number of arguments required by the function.",
        )
    ),

    DuplicateParameterException: DiagnosticDefinition(
        code=DiagnosticCode.DUPLICATE_PARAMETER,
        severity=DiagnosticSeverity.ERROR,
        title="duplicate parameter",
        help=(
            "Rename or remove the duplicate parameter from the function declaration.",
        ),
        notes=(
            "Each parameter in a function declaration must have a unique name.",
        )
    ),

    InvalidIndexTypeException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_INDEX_TYPE,
        severity=DiagnosticSeverity.ERROR,
        title="invalid index type",
        help=(
            "Use an index with a type supported by the value being indexed.",
        ),
        notes=(
            "The type of an index must be compatible with the type of value being indexed.",
        )
    ),

    ModuleNotFoundException: DiagnosticDefinition(
        code=DiagnosticCode.MODULE_NOT_FOUND,
        severity=DiagnosticSeverity.ERROR,
        title="module not found",
        help=(
            "Check that the module name is spelled correctly and that the module exists in the expected module path.",
        ),
        notes=(
            "Falx searches for modules relative to the current project or module path.",
        ),
    ),

    RecursionDepthExceededException: DiagnosticDefinition(
        code=DiagnosticCode.RECURSION_DEPTH_EXCEEDED,
        severity=DiagnosticSeverity.ERROR,
        title="recursion depth exceeded",
        help=(
            "Reduce the depth of recursive calls or restructure the code to avoid excessive recursion.",
        ),
        notes=(
            "Falx limits the maximum recursion depth to prevent excessive stack growth.",
        ),
    ),

    CircularModuleDependencyException: DiagnosticDefinition(
        code=DiagnosticCode.CIRCULAR_MODULE_DEPENDENCY,
        severity=DiagnosticSeverity.ERROR,
        title="circular module dependency detected",
        help=(
            "Remove the circular dependency between these modules.",
        ),
        notes=(
            "A module cannot be loaded while it is already being loaded.",
        )
    ),

    ObjectNotIterableException: DiagnosticDefinition(
        code=DiagnosticCode.OBJECT_NOT_ITERABLE,
        severity=DiagnosticSeverity.ERROR,
        title="object not iterable",
        help=(
            "Check that the value you are trying to iterate over is an iterable type.",
        ),
        notes=(
            "Only iterable values can be used in iteration.",
        )
    ),

    UnexpectedTypeException: DiagnosticDefinition(
        code=DiagnosticCode.UNEXPECTED_TYPE,
        severity=DiagnosticSeverity.ERROR,
        title="unexpected type",
        help=(
            "Check that the value has the expected type and that the expression evaluates to the intended value.",
        ),
        notes=(
            "This operation requires a value of a specific type.",
        )
    ),

    AssertionFailedException: DiagnosticDefinition(
        code=DiagnosticCode.ASSERTION_FAILED,
        severity=DiagnosticSeverity.ERROR,
        title="assertion failed",
        help=(
            "Check the condition passed to assert and make sure it evaluates to true.",
        ),
        notes=(
            "An assertion fails when its condition evaluates to false.",
        )
    ),

    UnexpectedEndOfInputException: DiagnosticDefinition(
        code=DiagnosticCode.UNEXPECTED_END_OF_INPUT,
        severity=DiagnosticSeverity.ERROR,
        title="unexpected end of input",
        help=(
            "Check for a missing closing delimiter such as `}`, `)`, or `]`.",
            "Check that the statement or expression is complete before the file ends.",
        ),
        notes=(
            "The parser reached the end of the input while still expecting more tokens.",
            "This usually means a block, group, or expression was opened but never closed.",
        )
    ),

    FalxRuntimeException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_OPERATION,
        severity=DiagnosticSeverity.ERROR,
        title="runtime error",
        help=(
            "Check the values and types involved in the expression that caused this error.",
        ),
        notes=(
            "An unexpected error occurred during execution.",
        )
    ),

    MissingFilenameException: DiagnosticDefinition(
        code=DiagnosticCode.MISSING_FILENAME,
        severity=DiagnosticSeverity.ERROR,
        title="missing filename",
        help=(
            "Provide a path to a `.flx` file after the flag.",
            "Example: `falx run main.flx`",
        ),
        notes=(
            "The `--file`, `-f`, and `run` flags require a filename argument.",
        )
    ),

    InvalidFileTypeException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_FILE_TYPE,
        severity=DiagnosticSeverity.ERROR,
        title="invalid file type",
        help=(
            "Make sure the file has a `.flx` extension.",
            "Example: `falx run main.flx`",
        ),
        notes=(
            "Falx can only execute files with the `.flx` extension.",
        )
    ),

    InvalidDefaultValueCreationException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_DEFAULT_CREATION,
        severity=DiagnosticSeverity.ERROR,
        title="invalid default value",
        help=(
            "Remove the non default value/s after parameters with default values.",
        ),
        notes=(
            "You cannot have a required parameter after an optional parameter.",
        )
    ),

    ImmutableValueException: DiagnosticDefinition(
        code=DiagnosticCode.IMMUTABLE_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="immutable value",
        help=(
            "Create a new value instead of modifying the existing one.",
            "Use an array if the contents need to change after creation.",
        ),
        notes=(
            "Some data types like tuples are immutable so their elements cannot be assigned, added or removed once it has been created.",
        )
    ),

    DestructureCountException: DiagnosticDefinition(
        code=DiagnosticCode.DESTRUCTURE_COUNT_MISMATCH,
        severity=DiagnosticSeverity.ERROR,
        title="destructuring count mismatch",
        help=(
            "Use exactly one variable for each element in the tuple.",
            "Check the size of the tuple with `.size` if it is not known ahead of time.",
        ),
        notes=(
            "Destructuring a tuple does not truncate values, so the counts must match.",
        )
    )
}