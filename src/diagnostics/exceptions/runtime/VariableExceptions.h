//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxRuntimeException.h"

class AssignToConstantException : public FalxRuntimeException {
public:
    explicit AssignToConstantException(const std::string& name, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot assign to constant '" + name + "'", location) {}
};

class DestructureCountException : public FalxRuntimeException {
public:
    explicit DestructureCountException(const int expected, const int actual, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot destructure a tuple with " + std::to_string(actual) + " element" + (actual == 1? "" : "s")
            + " into " + std::to_string(expected) + "variable" + (expected == 1? "" : "s"), location
        ) {}
};

class FunctionAlreadyExistsException : public FalxRuntimeException {
public:
    explicit FunctionAlreadyExistsException(const std::string& name, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Function '" + name + "' already exists", location) {}
};

class UndefinedVariableException : public FalxRuntimeException {
public:
    explicit UndefinedVariableException(const std::string& name, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Undefined variable '" + name + "'", location) {}
};

class VariableAlreadyExistsException : public FalxRuntimeException {
public:
    explicit VariableAlreadyExistsException(const std::string& name, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Variable '" + name + "' already exists", location) {}
};