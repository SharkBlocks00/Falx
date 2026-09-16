from src.diagnostics.DiagnosticCode import DiagnosticCode
from src.diagnostics.DiagnosticDefinition import DiagnosticDefinition
from src.diagnostics.DiagnosticSeverity import DiagnosticSeverity
from src.diagnostics.exceptions.lexer.syntax.InvalidCharacterException import InvalidCharacterException
from src.diagnostics.exceptions.lexer.syntax.InvalidEscapeSequenceException import InvalidEscapeSequenceException
from src.diagnostics.exceptions.lexer.syntax.UnterminatedStringException import UnterminatedStringException
from src.diagnostics.exceptions.parser.context.BreakOutsideFunction import BreakOutsideFunctionException
from src.diagnostics.exceptions.parser.context.ContinueOutsideFunctionException import ContinueOutsideFunctionException
from src.diagnostics.exceptions.parser.context.ReturnOutsideFunctionException import ReturnOutsideFunctionException
from src.diagnostics.exceptions.parser.syntax.InvalidAssignmentTargetException import InvalidAssignmentTargetException
from src.diagnostics.exceptions.parser.syntax.UnexpectedTokenException import UnexpectedTokenException
from src.diagnostics.exceptions.runtime.indexing.ArrayIndexInvalidException import ArrayIndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.IndexInvalidException import IndexInvalidException
from src.diagnostics.exceptions.runtime.indexing.StringIndexInvalidException import StringIndexInvalidException
from src.diagnostics.exceptions.runtime.methods.DuplicateParameterException import DuplicateParameterException
from src.diagnostics.exceptions.runtime.methods.InvalidArgumentCountException import InvalidArgumentCountException
from src.diagnostics.exceptions.runtime.methods.ObjectNotCallableException import ObjectNotCallableException
from src.diagnostics.exceptions.runtime.operations.CannotAddWithValueException import CannotAddWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotCompareToException import CannotCompareToException
from src.diagnostics.exceptions.runtime.operations.CannotConvertToTypeException import CannotConvertToTypeException
from src.diagnostics.exceptions.runtime.operations.CannotDivideByValueException import CannotDivideByValueException
from src.diagnostics.exceptions.runtime.operations.CannotEvaluateValueException import CannotEvaluateValueException
from src.diagnostics.exceptions.runtime.operations.CannotMinusFromValueException import CannotMinusFromValueException
from src.diagnostics.exceptions.runtime.operations.CannotModWithValueException import CannotModWithValueException
from src.diagnostics.exceptions.runtime.operations.CannotMultiplyByValue import CannotMultiplyByValueException
from src.diagnostics.exceptions.runtime.operations.DivisionByZeroException import DivisionByZeroException
from src.diagnostics.exceptions.runtime.properties.CannotAccessPropertyException import CannotAccessPropertyException
from src.diagnostics.exceptions.runtime.properties.CannotSetPropertyException import CannotSetPropertyException
from src.diagnostics.exceptions.runtime.indexing.ObjectIsNotIndexableException import ObjectIsNotIndexableException
from src.diagnostics.exceptions.runtime.variables.AssignToConstantException import AssignToConstantException
from src.diagnostics.exceptions.runtime.variables.UndefinedVariableException import UndefinedVariableException
from src.diagnostics.exceptions.runtime.variables.VariableAlreadyExistsException import VariableAlreadyExistsException

DIAGNOSTIC_REGISTRY: dict[..., DiagnosticDefinition] = {

    UndefinedVariableException: DiagnosticDefinition(
        code=DiagnosticCode.UNDEFINED_VARIABLE,
        severity=DiagnosticSeverity.ERROR,
        title="undefined variable",
        help=(
            "Check that the variable has been declared.",
            "Check that the variable name is spelled correctly."
        ),
        notes=("Variables must be declared before you can use them.",)
    ),

    DivisionByZeroException: DiagnosticDefinition(
        code=DiagnosticCode.DIVISION_BY_ZERO,
        severity=DiagnosticSeverity.ERROR,
        title="division by zero",
        help=(
            "Check the value of the divisor before performing the division.",
        ),
        notes=("You cannot divide by zero.",)
    ),

    ReturnOutsideFunctionException: DiagnosticDefinition(
        code=DiagnosticCode.RETURN_OUTSIDE_FUNCTION,
        severity=DiagnosticSeverity.ERROR,
        title="return outside function",
        help=(
            "Check you have not called return outside of a function.",
        ),
        notes=("Return can only be called inside of a function",)
    ),

    ContinueOutsideFunctionException: DiagnosticDefinition(
        code=DiagnosticCode.CONTINUE_OUTSIDE_LOOP,
        severity=DiagnosticSeverity.ERROR,
        title="continue outside loop",
        help=(
            "Check you have not called continue outside of a loop.",
        ),
        notes=("Continue can only be called inside of a loop",)
    ),

    BreakOutsideFunctionException: DiagnosticDefinition(
        code=DiagnosticCode.BREAK_OUTSIDE_LOOP,
        severity=DiagnosticSeverity.ERROR,
        title="break outside loop",
        help=(
            "Check you have not called break outside of a function or loop.",
        ),
        notes=("Break can only be called inside of a function or loop.",)
    ),

    InvalidAssignmentTargetException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ASSIGNMENT_TARGET,
        severity=DiagnosticSeverity.ERROR,
        title="invalid assignment target",
        help=(
            "Check the type of the assignment target.",
        ),
        notes=("Certain data types cannot be assigned to.",)
    ),

    UnexpectedTokenException: DiagnosticDefinition(
        code=DiagnosticCode.UNEXPECTED_TOKEN,
        severity=DiagnosticSeverity.ERROR,
        title="unexpected token",
        help=(
            "Check that your syntax is correct and not missing anything.",
        )
    ),

    InvalidCharacterException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_CHARACTER,
        severity=DiagnosticSeverity.ERROR,
        title="invalid character",
        help=(
            "Check that your syntax is correct and not missing anything.",
        )
    ),

    UnterminatedStringException: DiagnosticDefinition(
        code=DiagnosticCode.UNTERMINATED_STRING,
        severity=DiagnosticSeverity.ERROR,
        title="unterminated string",
        help=(
            "Check that you have put a \" or ' at the end of the string.",
        ),
        notes=("You must enclose strings with \" at either ends.",)
    ),

    InvalidEscapeSequenceException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ESCAPE_SEQUENCE,
        severity=DiagnosticSeverity.ERROR,
        title="invalid escape sequence",
        help=(
            "Check that your escape sequence is valid.",
        ),
        notes=("Valid escape sequences include the following: '\\n', '\\t', '\\r', '\\f', '\\b', '\\\"', '\\\\'.",)
    ),

    AssignToConstantException: DiagnosticDefinition(
        code=DiagnosticCode.ASSIGN_TO_CONSTANT,
        severity=DiagnosticSeverity.ERROR,
        title="assign to constant",
        help=(
            "Check that you are not attempting to assign a value to a `const` variable.",
        ),
        notes=("You cannot assign a new value to a variable declared using `const`.",)
    ),

    VariableAlreadyExistsException: DiagnosticDefinition(
        code=DiagnosticCode.VARIABLE_ALREADY_EXISTS,
        severity=DiagnosticSeverity.ERROR,
        title="variable already exists",
        help=(
            "Check that you are not re-declaring a variable using `let` or `const`.",
        ),
        notes=("You cannot declare a variable that has already been declared using `let` or `const`.",
               "If you are wanting to assign a value to a variable, simply remove `let` or `const`."
               )
    ),

    CannotMultiplyByValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_MULTIPLY_BY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot multiply by value",
        help=(
            "Check you are performing a valid multiplication.",
        ),
        notes=("Not all data types can be multiplied together, for example, a string and a string.",)
    ),

    CannotDivideByValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_DIVIDE_BY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot divide by value",
        help=(
            "Check you are performing a valid division.",
        ),
        notes=("Not all data types can be divided by another, for example, a string and a string.",)
    ),

    CannotAddWithValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_ADD_WITH_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot add with value",
        help=(
            "Check you are performing a valid addition.",
        ),
        notes=("Not all data types can be added, for example, an array and an integer.",)
    ),

    CannotModWithValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_MOD_WITH_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot mod with value",
        help=(
            "Check you are performing a valid modulus.",
        ),
        notes=("Not all data types can be used with modulus (`%`), for example, an integer and a string.",)
    ),

    CannotCompareToException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_COMPARE_TO,
        severity=DiagnosticSeverity.ERROR,
        title="cannot compare to",
        help=(
            "Check you are comparing two comparable items.",
        ),
    ),

    CannotConvertToTypeException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_CONVERT_TO_TYPE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot convert to type",
        help=(
            "Check that the values you are trying to cast can be casted to that type.",
        ),
        notes=("Not all data types support casting.",
               "Not all data types can be casted to every individual type.")
    ),

    CannotAccessPropertyException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_ACCESS_PROPERTY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot access property",
        help=(
            "Check that the property you are trying to access exists.",
        ),
        notes=("Check the data type of the object you are trying to get the property of.",)
    ),

    ObjectIsNotIndexableException: DiagnosticDefinition(
        code=DiagnosticCode.OBJECT_IS_NOT_INDEXABLE,
        severity=DiagnosticSeverity.ERROR,
        title="object is not indexable",
        help=(
            "The object you are trying to index is not indexable.",
        ),
        notes=("Check the type of the object you are trying to index.",)
    ),

    CannotSetPropertyException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_SET_PROPERTY,
        severity=DiagnosticSeverity.ERROR,
        title="cannot set property",
        help=(
            "Check that the property you are trying to set exists and is modifiable.",
        ),
    ),

    CannotEvaluateValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_EVALUATE_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot evaluate value",
        help=(
            "Check the operator of the evaluation and make sure it is valid.",
        ),
        notes=("Valid mathematical operators include the following: '+', '-', '*', '/', '%'.",
               "Valid comparison operators include the following: '>', '>=', '<', '<=', '==', '!='.")
    ),

    IndexInvalidException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_INDEX,
        severity=DiagnosticSeverity.ERROR,
        title="invalid index",
        help=(
            "Check the index is valid.",
        ),
    ),

    ArrayIndexInvalidException: DiagnosticDefinition(
        code=DiagnosticCode.ARRAY_INVALID_INDEX,
        severity=DiagnosticSeverity.ERROR,
        title="invalid index on array",
        help=(
            "Check the index is within the lower and upper bounds of the array.",
        ),
    ),

    StringIndexInvalidException: DiagnosticDefinition(
        code=DiagnosticCode.STRING_INVALID_INDEX,
        severity=DiagnosticSeverity.ERROR,
        title="invalid index on string",
        help=(
            "Check the index is within the lower and upper bounds of the string.",
        ),
    ),

    CannotMinusFromValueException: DiagnosticDefinition(
        code=DiagnosticCode.CANNOT_MINUS_FROM_VALUE,
        severity=DiagnosticSeverity.ERROR,
        title="cannot minus from value",
        help=(
            "Check you are performing a valid subtraction.",
        ),
    ),

    ObjectNotCallableException: DiagnosticDefinition(
        code=DiagnosticCode.OBJECT_NOT_CALLABLE,
        severity=DiagnosticSeverity.ERROR,
        title="object not callable",
        help=(
            "Check the object you are trying to call is callable.",
        ),
    ),

    InvalidArgumentCountException: DiagnosticDefinition(
        code=DiagnosticCode.INVALID_ARGUMENT_COUNT,
        severity=DiagnosticSeverity.ERROR,
        title="invalid argument count",
        help=(
            "Check you are passing in the correct number of arguments.",
        )
    ),

    DuplicateParameterException: DiagnosticDefinition(
        code=DiagnosticCode.DUPLICATE_PARAMETER,
        severity=DiagnosticSeverity.ERROR,
        title="duplicate parameter",
        help=(
            "Check that there is not more than one parameter of the same name in your function declaration",
        )
    )

}