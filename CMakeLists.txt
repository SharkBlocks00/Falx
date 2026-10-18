cmake_minimum_required(VERSION 4.1)
project(CFalx)

set(CMAKE_CXX_STANDARD 23)

add_executable(CFalx src/main.cpp
        ../Falx-cpp/src/tokens/Token.cpp
        ../Falx-cpp/src/tokens/Token.h
        ../Falx-cpp/src/ast/SourceLocation.cpp
        ../Falx-cpp/src/ast/SourceLocation.h
        ../Falx-cpp/src/tokens/TokenKind.h
        ../Falx-cpp/src/lexer/Lexer.cpp
        ../Falx-cpp/src/lexer/Lexer.h
        ../Falx-cpp/src/ast/Expression.h
        ../Falx-cpp/src/runtime/environment/Environment.cpp
        ../Falx-cpp/src/runtime/environment/Environment.h
        ../Falx-cpp/src/ast/Variable.h
        ../Falx-cpp/src/runtime/objects/FalxValue.cpp
        ../Falx-cpp/src/runtime/objects/FalxValue.h
        ../Falx-cpp/src/packages/Callable.h
        ../Falx-cpp/src/packages/NativeFunction.h
        ../Falx-cpp/src/runtime/interpreter/Interpreter.cpp
        ../Falx-cpp/src/runtime/interpreter/Interpreter.h
        ../Falx-cpp/src/ast/expressions/literals/ArrayLiteral.h
        ../Falx-cpp/src/runtime/objects/advanced/FalxArray.cpp
        ../Falx-cpp/src/runtime/objects/advanced/FalxArray.h
        ../Falx-cpp/src/runtime/objects/FalxIterable.h
        ../Falx-cpp/src/runtime/objects/FalxBoolean.h
        ../Falx-cpp/src/ast/expressions/literals/BooleanLiteral.h
        ../Falx-cpp/src/runtime/objects/advanced/FalxMap.cpp
        ../Falx-cpp/src/runtime/objects/advanced/FalxMap.h
        ../Falx-cpp/src/runtime/objects/advanced/FalxString.cpp
        ../Falx-cpp/src/runtime/objects/advanced/FalxString.h
        ../Falx-cpp/src/ast/expressions/literals/StringLiteral.h
        ../Falx-cpp/src/runtime/objects/FalxNull.h
        ../Falx-cpp/src/ast/expressions/literals/NullLiteral.h
        src/ast/expressions/literals/MapLiteral.h
        src/runtime/objects/advanced/FalxNumber.cpp
        src/runtime/objects/advanced/FalxNumber.h
)
