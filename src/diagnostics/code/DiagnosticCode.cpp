//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "DiagnosticCode.hpp"

std::string_view toString(const DiagnosticCode code) {
    switch (code) {
        case DiagnosticCode::UNDEFINED_VARIABLE: return "E1000";
        case DiagnosticCode::UNDEFINED_PROPERTY: return "E1001";
        case DiagnosticCode::INVALID_OPERATION: return "E1002";
        case DiagnosticCode::DIVISION_BY_ZERO: return "E1003";
        case DiagnosticCode::ASSIGN_TO_CONSTANT: return "E1004";
        case DiagnosticCode::VARIABLE_ALREADY_EXISTS: return "E1005";
        case DiagnosticCode::RECURSION_DEPTH_EXCEEDED: return "E1024";
        case DiagnosticCode::OBJECT_NOT_ITERABLE: return "E1025";
        case DiagnosticCode::ASSERTION_FAILED: return "E1027";
        case DiagnosticCode::IMMUTABLE_VALUE: return "E1028";
        case DiagnosticCode::DESTRUCTURE_COUNT_MISMATCH: return "E1029";
        case DiagnosticCode::FUNCTION_ALREADY_EXISTS: return "E1031";
        case DiagnosticCode::EXPECTED_VALUE: return "E1032";
        case DiagnosticCode::CANNOT_HASH_OBJECT: return "E1033";
        case DiagnosticCode::THROWN_EXCEPTION: return "E1035";

        // Type errors
        case DiagnosticCode::UNEXPECTED_TYPE: return "E1026";
        case DiagnosticCode::TYPE_CHECK_ERROR: return "E1036";

        // Static type errors
        case DiagnosticCode::TYPE_MISMATCH: return "E7000";
        case DiagnosticCode::NULLABLE_VALUE: return "E7001";
        case DiagnosticCode::UNKNOWN_TYPE: return "E7002";
        case DiagnosticCode::INVALID_TYPE_ANNOTATION: return "E7003";
        case DiagnosticCode::MISSING_RETURN: return "E7004";
        case DiagnosticCode::VOID_VALUE: return "E7005";

        // Operational errors
        case DiagnosticCode::CANNOT_MULTIPLY_BY: return "E1006";
        case DiagnosticCode::CANNOT_DIVIDE_BY: return "E1007";
        case DiagnosticCode::CANNOT_ADD_WITH_VALUE: return "E1008";
        case DiagnosticCode::CANNOT_MOD_WITH_VALUE: return "E1009";
        case DiagnosticCode::CANNOT_COMPARE_TO: return "E1010";
        case DiagnosticCode::CANNOT_CONVERT_TO_TYPE: return "E1011";
        case DiagnosticCode::CANNOT_EVALUATE_VALUE: return "E1015";
        case DiagnosticCode::CANNOT_MINUS_FROM_VALUE: return "E1019";
        case DiagnosticCode::CANNOT_INVERT_VALUE: return "E1030";

        // Property errors
        case DiagnosticCode::CANNOT_ACCESS_PROPERTY: return "E1012";
        case DiagnosticCode::CANNOT_SET_PROPERTY: return "E1014";

        // Indexing errors
        case DiagnosticCode::OBJECT_IS_NOT_INDEXABLE: return "E1013";
        case DiagnosticCode::INVALID_INDEX: return "E1016";
        case DiagnosticCode::ARRAY_INVALID_INDEX: return "E1017";
        case DiagnosticCode::STRING_INVALID_INDEX: return "E1018";
        case DiagnosticCode::INVALID_INDEX_TYPE: return "E1023";
        case DiagnosticCode::MAP_INVALID_INDEX: return "E1034";

        // Method errors
        case DiagnosticCode::OBJECT_NOT_CALLABLE: return "E1020";
        case DiagnosticCode::INVALID_ARGUMENT_COUNT: return "E1021";
        case DiagnosticCode::DUPLICATE_PARAMETER: return "E1022";

        // Parser errors
        case DiagnosticCode::SYNTAX_ERROR: return "E2000";
        case DiagnosticCode::UNEXPECTED_TOKEN: return "E2001";
        case DiagnosticCode::RETURN_OUTSIDE_FUNCTION: return "E2002";
        case DiagnosticCode::CONTINUE_OUTSIDE_LOOP: return "E2003";
        case DiagnosticCode::BREAK_OUTSIDE_LOOP: return "E2004";
        case DiagnosticCode::INVALID_ASSIGNMENT_TARGET: return "E2005";
        case DiagnosticCode::UNEXPECTED_END_OF_INPUT: return "E2006";
        case DiagnosticCode::INVALID_DEFAULT_CREATION: return "E2007";

        // Lexer errors
        case DiagnosticCode::INVALID_CHARACTER: return "E3000";
        case DiagnosticCode::UNTERMINATED_STRING: return "E3001";
        case DiagnosticCode::INVALID_ESCAPE_SEQUENCE: return "E3002";

        // Module errors
        case DiagnosticCode::MODULE_NOT_FOUND: return "E4000";
        case DiagnosticCode::CIRCULAR_MODULE_DEPENDENCY: return "E4001";
        case DiagnosticCode::EXPORT_MUTABLE: return "E4002";

        // CLI errors
        case DiagnosticCode::MISSING_FILENAME: return "E5000";
        case DiagnosticCode::INVALID_FILE_TYPE: return "E5001";

        // File errors
        case DiagnosticCode::FILE_NOT_FOUND: return "E6000";
        case DiagnosticCode::PERMISSION_DENIED: return "E6001";
        case DiagnosticCode::FILE_ALREADY_EXISTS: return "E6002";
        case DiagnosticCode::NOT_A_FILE: return "E6003";
        case DiagnosticCode::NOT_A_DIRECTORY: return "E6004";
        case DiagnosticCode::INVALID_PATH: return "E6005";
        case DiagnosticCode::IO_ERROR: return "E6006";
        case DiagnosticCode::FILE_CLOSED: return "E6007";
        case DiagnosticCode::INVALID_FILE_MODE: return "E6008";

        default: return "UNKNOWN";
    }
}
