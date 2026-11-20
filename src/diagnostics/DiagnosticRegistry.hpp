#pragma once

// Generated from the Falx Python diagnostic registry.
// Assumes the C++ exception headers follow the same directory layout,
// and DiagnosticDefinition has: code, severity, title, help, and notes.

#include "code/DiagnosticCode.hpp"
#include "DiagnosticSeverity.h"
#include "DiagnosticDefinition.h"
#include "exceptions/CLIExceptions.h"
#include "exceptions/files/IOException.h"
#include "exceptions/files/FileExceptions.h"
#include "exceptions/files/ModeExceptions.h"
#include "exceptions/files/PathExceptions.h"
#include "exceptions/LexerExceptions.h"
#include "exceptions/ParserExceptions.h"
#include "exceptions/runtime/FalxRuntimeException.h"
#include "exceptions/runtime/IndexExceptions.h"
#include "exceptions/runtime/MethodExceptions.h"
#include "exceptions/runtime/ModuleExceptions.h"
#include "exceptions/runtime/OperationExceptions.h"
#include "exceptions/runtime/PropertyExceptions.h"
#include "exceptions/runtime/TypingExceptions.h"
#include "exceptions/runtime/VariableExceptions.h"
#include "exceptions/TypeExceptions.h"

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

class DiagnosticRegistry {
public:
    [[nodiscard]] static const DiagnosticDefinition* find(const std::type_info& type) {
        const auto& registry = definitions();
        const auto it = registry.find(std::type_index(type));
        return it == registry.end() ? nullptr : &it->second;
    }

    template <typename T>
    [[nodiscard]] static const DiagnosticDefinition* find() {
        return find(typeid(T));
    }

private:
    [[nodiscard]] static const std::unordered_map<std::type_index, DiagnosticDefinition>& definitions() {
        static const std::unordered_map<std::type_index, DiagnosticDefinition> registry = {
            {
                std::type_index(typeid(UndefinedVariableException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::UNDEFINED_VARIABLE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "undefined variable",
                    .help = {
                        "Check that the variable has been declared before it is used.",
                        "Check that the variable name is spelled correctly.",
                    },
                    .notes = {
                        "Variables must be declared using `let` or `const` before they can be used.",
                    }
                }
            },
            {
                std::type_index(typeid(DivisionByZeroException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::DIVISION_BY_ZERO,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "division by zero",
                    .help = {
                        "Check the value being used as the divisor.",
                        "Make sure the divisor cannot evaluate to zero before performing the division.",
                    },
                    .notes = {
                        "Division by zero is not a valid operation in Falx.",
                    }
                }
            },
            {
                std::type_index(typeid(ReturnOutsideFunctionException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::RETURN_OUTSIDE_FUNCTION,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "return outside function",
                    .help = {
                        "Move the `return` statement inside a function declaration.",
                    },
                    .notes = {
                        "The `return` statement can only be used while executing a function.",
                    }
                }
            },
            {
                std::type_index(typeid(ContinueOutsideLoopException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CONTINUE_OUTSIDE_LOOP,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "continue outside loop",
                    .help = {
                        "Move the `continue` statement inside a loop.",
                    },
                    .notes = {
                        "The `continue` statement can only be used inside a loop.",
                    }
                }
            },
            {
                std::type_index(typeid(BreakOutsideLoopException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::BREAK_OUTSIDE_LOOP,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "break outside loop",
                    .help = {
                        "Move the `break` statement inside a loop.",
                    },
                    .notes = {
                        "The `break` statement can only be used inside a loop.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidAssignmentTargetException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_ASSIGNMENT_TARGET,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid assignment target",
                    .help = {
                        "Check that the left-hand side of the assignment is a valid assignment target.",
                    },
                    .notes = {
                        "Only assignable variables, properties, and other supported targets can appear on the left-hand side of an assignment.",
                    }
                }
            },
            {
                std::type_index(typeid(UnexpectedTokenException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::UNEXPECTED_TOKEN,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "unexpected token",
                    .help = {
                        "Check the surrounding syntax for a missing, extra, or incorrectly placed token.",
                        "Check that the statement follows the expected Falx syntax.",
                    },
                    .notes = {
                    }
                }
            },
            {
                std::type_index(typeid(InvalidCharacterException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_CHARACTER,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid character",
                    .help = {
                        "Remove the invalid character or replace it with a valid Falx character.",
                        "Check that the character is not the result of a typing or encoding error.",
                    },
                    .notes = {
                        "The lexer encountered a character that is not valid in the current context.",
                    }
                }
            },
            {
                std::type_index(typeid(UnterminatedStringException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::UNTERMINATED_STRING,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "unterminated string",
                    .help = {
                        "Add the missing closing quote to the string.",
                        "Check that an escaped quote has not been unintentionally used or omitted.",
                    },
                    .notes = {
                        "String literals must begin and end with matching quotation marks.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidEscapeSequenceException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_ESCAPE_SEQUENCE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid escape sequence",
                    .help = {
                        "Replace the invalid escape sequence with a supported escape sequence.",
                    },
                    .notes = {
                        "Supported escape sequences include `\\n`, `\\t`, `\\r`, `\\f`, `\\b`, `\\\"`, and `\\\\`.",
                    }
                }
            },
            {
                std::type_index(typeid(AssignToConstantException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::ASSIGN_TO_CONSTANT,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "assign to constant",
                    .help = {
                        "Remove the assignment or declare the variable using `let` if its value needs to change.",
                    },
                    .notes = {
                        "Variables declared using `const` cannot be assigned a new value after declaration.",
                    }
                }
            },
            {
                std::type_index(typeid(VariableAlreadyExistsException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::VARIABLE_ALREADY_EXISTS,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "variable already exists",
                    .help = {
                        "Choose a different variable name or assign a new value to the existing variable.",
                        "If you are assigning to an existing variable, remove `let` or `const` from the declaration.",
                    },
                    .notes = {
                        "A variable cannot be declared more than once in the same environment.",
                    }
                }
            },
            {
                std::type_index(typeid(FunctionAlreadyExistsException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::FUNCTION_ALREADY_EXISTS,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "function already exists",
                    .help = {
                        "Choose a different function name than the one you are assigning to.",
                    },
                    .notes = {
                        "A function cannot be declared more than once in the same environment.",
                        "A function is immutable, so you must declare a new function instead of assigning a new value.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotMultiplyByValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_MULTIPLY_BY,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot multiply by value",
                    .help = {
                        "Check that both operands support multiplication with each other.",
                    },
                    .notes = {
                        "The `*` operator only supports combinations of data types that define multiplication.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotDivideByValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_DIVIDE_BY,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot divide by value",
                    .help = {
                        "Check that both operands support division with each other.",
                        "If the divisor is valid but evaluates to zero, check for a division-by-zero error instead.",
                    },
                    .notes = {
                        "The `/` operator only supports combinations of data types that define division.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotAddWithException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_ADD_WITH_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot add with value",
                    .help = {
                        "Check that both operands support addition with each other.",
                    },
                    .notes = {
                        "The `+` operator only supports combinations of data types that define addition.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotModWithValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_MOD_WITH_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot mod with value",
                    .help = {
                        "Check that both operands support the modulus operation.",
                    },
                    .notes = {
                        "The `%` operator only supports combinations of data types that define modulus.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotCompareToException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_COMPARE_TO,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot compare to",
                    .help = {
                        "Check that the values being compared support the selected comparison operator.",
                    },
                    .notes = {
                        "Comparison operators can only be used with values that support comparison.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotConvertToTypeException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_CONVERT_TO_TYPE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot convert to type",
                    .help = {
                        "Check that the value can be converted to the requested type.",
                        "Use a type conversion that is supported by the value and target type.",
                    },
                    .notes = {
                        "Not every data type can be converted to every other data type.",
                        "Type conversion is only supported where Falx defines a valid conversion.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotAccessPropertyException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_ACCESS_PROPERTY,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot access property",
                    .help = {
                        "Check that the property exists on the value being accessed.",
                        "Check that the value supports property access.",
                    },
                    .notes = {
                        "A property can only be accessed when the object's type provides that property.",
                    }
                }
            },
            {
                std::type_index(typeid(ObjectIsNotIndexableException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::OBJECT_IS_NOT_INDEXABLE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "object is not indexable",
                    .help = {
                        "Check that the value supports indexing with `[]`.",
                    },
                    .notes = {
                        "Only indexable values can be accessed using an index.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotSetPropertyException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_SET_PROPERTY,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot set property",
                    .help = {
                        "Check that the property exists and can be modified.",
                        "Check that the value allows the property to be changed.",
                    },
                    .notes = {
                        "A property cannot be assigned when it does not exist or is not modifiable.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotEvaluateValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_EVALUATE_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot evaluate value",
                    .help = {
                        "Check that the operator is valid for the values being evaluated.",
                        "Check that the expression uses a supported operator.",
                    },
                    .notes = {
                        "Falx supports arithmetic operators such as `+`, `-`, `*`, `/`, and `%`.",
                        "Falx also supports comparison operators such as `>`, `>=`, `<`, `<=`, `==`, and `!=`.",
                    }
                }
            },
            {
                std::type_index(typeid(IndexInvalidException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_INDEX,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid index",
                    .help = {
                        "Check that the index is valid for the value being accessed.",
                    },
                    .notes = {
                        "An index must be valid for the type and size of the value being indexed.",
                    }
                }
            },
            {
                std::type_index(typeid(ArrayIndexInvalidException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::ARRAY_INVALID_INDEX,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid index on array",
                    .help = {
                        "Check that the index is within the valid range of the array.",
                    },
                    .notes = {
                        "An array index must refer to an element that exists within the array.",
                    }
                }
            },
            {
                std::type_index(typeid(MapIndexInvalidException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::MAP_INVALID_INDEX,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid index on map",
                    .help = {
                        "Check that the index/key is within the valid range or keyset of the map.",
                    },
                    .notes = {
                        "A map index must refer to an element that exists within the map",
                    }
                }
            },
            {
                std::type_index(typeid(StringIndexInvalidException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::STRING_INVALID_INDEX,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid index on string",
                    .help = {
                        "Check that the index is within the valid range of the string.",
                    },
                    .notes = {
                        "A string index must refer to a character that exists within the string.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotMinusFromValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_MINUS_FROM_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot subtract from value",
                    .help = {
                        "Check that the operands support subtraction with each other.",
                    },
                    .notes = {
                        "The `-` operator only supports combinations of data types that define subtraction.",
                    }
                }
            },
            {
                std::type_index(typeid(ObjectNotCallableException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::OBJECT_NOT_CALLABLE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "object not callable",
                    .help = {
                        "Check that the value you are trying to call is a function or another callable value.",
                    },
                    .notes = {
                        "Only callable values can be followed by a function call.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidArgumentCountException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_ARGUMENT_COUNT,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid argument count",
                    .help = {
                        "Check the function declaration and provide the expected number of arguments.",
                    },
                    .notes = {
                        "A function call must provide the number of arguments required by the function.",
                    }
                }
            },
            {
                std::type_index(typeid(DuplicateParameterException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::DUPLICATE_PARAMETER,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "duplicate parameter",
                    .help = {
                        "Rename or remove the duplicate parameter from the function declaration.",
                    },
                    .notes = {
                        "Each parameter in a function declaration must have a unique name.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidIndexTypeException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_INDEX_TYPE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid index type",
                    .help = {
                        "Use an index with a type supported by the value being indexed.",
                    },
                    .notes = {
                        "The type of an index must be compatible with the type of value being indexed.",
                    }
                }
            },
            {
                std::type_index(typeid(ModuleNotFoundException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::MODULE_NOT_FOUND,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "module not found",
                    .help = {
                        "Check that the module name is spelled correctly and that the module exists in the expected module path.",
                    },
                    .notes = {
                        "Falx searches for modules relative to the current project or module path.",
                    }
                }
            },
            {
                std::type_index(typeid(RecursionDepthExceededException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::RECURSION_DEPTH_EXCEEDED,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "recursion depth exceeded",
                    .help = {
                        "Reduce the depth of recursive calls or restructure the code to avoid excessive recursion.",
                    },
                    .notes = {
                        "Falx limits the maximum recursion depth to prevent excessive stack growth.",
                    }
                }
            },
            {
                std::type_index(typeid(CircularModuleDependencyException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CIRCULAR_MODULE_DEPENDENCY,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "circular module dependency detected",
                    .help = {
                        "Remove the circular dependency between these modules.",
                    },
                    .notes = {
                        "A module cannot be loaded while it is already being loaded.",
                    }
                }
            },
            {
                std::type_index(typeid(ObjectNotIterableException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::OBJECT_NOT_ITERABLE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "object not iterable",
                    .help = {
                        "Check that the value you are trying to iterate over is an iterable type.",
                    },
                    .notes = {
                        "Only iterable values can be used in iteration.",
                    }
                }
            },
            {
                std::type_index(typeid(UnexpectedTypeException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::UNEXPECTED_TYPE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "unexpected type",
                    .help = {
                        "Check that the value has the expected type and that the expression evaluates to the intended value.",
                    },
                    .notes = {
                        "This operation requires a value of a specific type.",
                    }
                }
            },
            {
                std::type_index(typeid(AssertionFailedException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::ASSERTION_FAILED,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "assertion failed",
                    .help = {
                        "Check the condition passed to assert and make sure it evaluates to true.",
                    },
                    .notes = {
                        "An assertion fails when its condition evaluates to false.",
                    }
                }
            },
            {
                std::type_index(typeid(UnexpectedEndOfInputException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::UNEXPECTED_END_OF_INPUT,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "unexpected end of input",
                    .help = {
                        "Check for a missing closing delimiter such as `}`, `)`, or `]`.",
                        "Check that the statement or expression is complete before the file ends.",
                    },
                    .notes = {
                        "The parser reached the end of the input while still expecting more tokens.",
                        "This usually means a block, group, or expression was opened but never closed.",
                    }
                }
            },
            {
                std::type_index(typeid(FalxRuntimeException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_OPERATION,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "runtime error",
                    .help = {
                        "Check the values and types involved in the expression that caused this error.",
                    },
                    .notes = {
                        "An unexpected error occurred during execution.",
                    }
                }
            },
            {
                std::type_index(typeid(MissingFilenameException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::MISSING_FILENAME,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "missing filename",
                    .help = {
                        "Provide a path to a `.flx` file after the flag.",
                        "Example: `falx run main.flx`",
                    },
                    .notes = {
                        "The `--file`, `-f`, and `run` flags require a filename argument.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidFileTypeException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_FILE_TYPE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid file type",
                    .help = {
                        "Make sure the file has a `.flx` extension.",
                        "Example: `falx run main.flx`",
                    },
                    .notes = {
                        "Falx can only execute files with the `.flx` extension.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidDefaultValueCreationException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_DEFAULT_CREATION,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid default value",
                    .help = {
                        "Remove the non default value/s after parameters with default values.",
                    },
                    .notes = {
                        "You cannot have a required parameter after an optional parameter.",
                    }
                }
            },
            {
                std::type_index(typeid(ImmutableValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::IMMUTABLE_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "immutable value",
                    .help = {
                        "Create a new value instead of modifying the existing one.",
                        "Use an array if the contents need to change after creation.",
                    },
                    .notes = {
                        "Some data types like tuples are immutable so their elements cannot be assigned, added or removed once it has been created.",
                    }
                }
            },
            {
                std::type_index(typeid(DestructureCountException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::DESTRUCTURE_COUNT_MISMATCH,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "destructuring count mismatch",
                    .help = {
                        "Use exactly one variable for each element in the tuple.",
                        "Check the size of the tuple with `.size` if it is not known ahead of time.",
                    },
                    .notes = {
                        "Destructuring a tuple does not truncate values, so the counts must match.",
                    }
                }
            },
            {
                std::type_index(typeid(CannotInvertValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_INVERT_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot invert value",
                    .help = {
                        "Check that the type of the value you are inverting can be inverted",
                    },
                    .notes = {
                    }
                }
            },
            {
                std::type_index(typeid(ExpectedValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::EXPECTED_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "expected value",
                    .help = {
                        "Check that you are supplying the correct arguments",
                    },
                    .notes = {
                    }
                }
            },
            {
                std::type_index(typeid(CannotHashObjectException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::CANNOT_HASH_OBJECT,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot hash object",
                    .help = {
                        "Make sure you are not using a mutable object as a key",
                    },
                    .notes = {
                    }
                }
            },
            {
                std::type_index(typeid(FileNotFoundException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::FILE_NOT_FOUND,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "file not found",
                    .help = {
                        "Check that the file path is correct and that the file exists.",
                        "Check that the file has not been moved, renamed, or deleted.",
                    },
                    .notes = {
                        "The requested file or path does not exist.",
                    }
                }
            },
            {
                std::type_index(typeid(PermissionDeniedException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::PERMISSION_DENIED,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "permission denied",
                    .help = {
                        "Check that you have permission to access the file or directory.",
                        "Check that the file is not restricted by the operating system or another security mechanism.",
                    },
                    .notes = {
                        "The operating system denied access to the requested file or directory.",
                    }
                }
            },
            {
                std::type_index(typeid(FileAlreadyExistsException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::FILE_ALREADY_EXISTS,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "file already exists",
                    .help = {
                        "Choose a different file path or use an operation that allows an existing file to be replaced.",
                    },
                    .notes = {
                        "The requested operation requires a file to not exist, but a file already exists at that path.",
                    }
                }
            },
            {
                std::type_index(typeid(NotAFileException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::NOT_A_FILE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "path is not a file",
                    .help = {
                        "Check that the path refers to a file rather than a directory or another type of filesystem entry.",
                    },
                    .notes = {
                        "The specified path exists, but it does not refer to a regular file.",
                    }
                }
            },
            {
                std::type_index(typeid(NotADirectoryException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::NOT_A_DIRECTORY,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "path is not a directory",
                    .help = {
                        "Check that the path refers to a directory.",
                        "Check that the path has not been replaced by a file or another filesystem entry.",
                    },
                    .notes = {
                        "The specified path exists, but it does not refer to a directory.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidPathException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_PATH,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid file path",
                    .help = {
                        "Check that the file path is correctly formatted and contains only valid path components.",
                        "Check for invalid characters or malformed path syntax.",
                    },
                    .notes = {
                        "The supplied path is not valid for the current filesystem or operating system.",
                    }
                }
            },
            {
                std::type_index(typeid(IOException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::IO_ERROR,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "file input/output error",
                    .help = {
                        "Check that the file or directory is accessible and that the operation can be completed.",
                        "Check the underlying error information for more details.",
                    },
                    .notes = {
                        "The operating system reported an error while performing a file input/output operation.",
                    }
                }
            },
            {
                std::type_index(typeid(FileClosedException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::FILE_CLOSED,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "file is closed",
                    .help = {
                        "Open the file before attempting to read from or write to it.",
                        "Check that the file has not already been closed.",
                    },
                    .notes = {
                        "File operations cannot be performed on a file handle after it has been closed.",
                    }
                }
            },
            {
                std::type_index(typeid(InvalidFileModeException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_FILE_MODE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid file mode",
                    .help = {
                        "Check that the file mode is valid and supported by Falx.",
                        "Use a mode appropriate for the intended file operation.",
                    },
                    .notes = {
                        "The supplied file mode is not valid or supported for opening a file.",
                    }
                }
            },
            {
                std::type_index(typeid(ExportMutableValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::EXPORT_MUTABLE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "cannot export mutable value",
                    .help = {
                        "Export the value as a constant or expose it through a function instead.",
                        "Keep mutable state private to the module and provide functions to access or modify it.",
                    },
                    .notes = {
                        "Mutable values cannot be exported.",
                    }
                }
            },
            {
                std::type_index(typeid(TypeCheckError)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::TYPE_CHECK_ERROR,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "type check error",
                    .help = {
                        "Check the types of the values involved in this expression.",
                    },
                    .notes = {
                        "Falx checks types before running a program; see TYPING.md for the rules.",
                    }
                }
            },
            {
                std::type_index(typeid(TypeMismatchException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::TYPE_MISMATCH,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "type mismatch",
                    .help = {
                        "Change the value, or change the declared type so that it accepts the value.",
                        "Use '??' to give a nullable value a default, or '?' to make the declared type nullable.",
                    },
                    .notes = {
                        "A value can only be used where its type is assignable to the expected type.",
                    }
                }
            },
            {
                std::type_index(typeid(NullableValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::NULLABLE_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "value may be null",
                    .help = {
                        "Use '?.' instead of '.' to access a member only when the value is not null.",
                        "Use '??' to provide a default, or check the value with '!= null' first.",
                    },
                    .notes = {
                        "A type written 'T?' may hold null, so it cannot be used as a plain 'T' until null has been ruled out.",
                    }
                }
            },
            {
                std::type_index(typeid(UnknownTypeException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::UNKNOWN_TYPE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "unknown type",
                    .help = {
                        "Check the spelling of the type name.",
                        "Types are the builtin types (int, float, bool, String, any, void, null, Map<K, V>) and structs declared before use.",
                    },
                    .notes = {
                    }
                }
            },
            {
                std::type_index(typeid(InvalidTypeAnnotationException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::INVALID_TYPE_ANNOTATION,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "invalid type annotation",
                    .help = {
                        "Check the shape of the annotation, e.g. 'Map<String, int>', '(int, int) -> int' or 'String?'.",
                    },
                    .notes = {
                    }
                }
            },
            {
                std::type_index(typeid(MissingReturnException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::MISSING_RETURN,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "missing return",
                    .help = {
                        "Return a value on every path through the function, or declare a nullable return type such as 'String?'.",
                    },
                    .notes = {
                    }
                }
            },
            {
                std::type_index(typeid(VoidValueException)),
                DiagnosticDefinition{
                    .code = DiagnosticCode::VOID_VALUE,
                    .severity = DiagnosticSeverity::ERROR,
                    .title = "void value used",
                    .help = {
                        "Only use the result of functions that return a value.",
                    },
                    .notes = {
                    }
                }
            },
        };

        return registry;
    }
};
