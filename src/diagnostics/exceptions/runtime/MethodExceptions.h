//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxRuntimeException.h"

class DuplicateParameterException : public FalxRuntimeException {
public:
    explicit DuplicateParameterException(const std::string& parameter, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Duplicate parameter '" + parameter + "'") {}
};

class InvalidArgumentCountException : public FalxRuntimeException {
public:
    explicit InvalidArgumentCountException(const int count, const int expected, const std::optional<SourceLocation>& location = std::nullopt, const std::optional<std::string>& message = std::nullopt)
        : FalxRuntimeException((message.has_value()? message.value() : "Expected " + std::to_string(expected) + " arguments, but got " + std::to_string(count)), location) {
    }
};

class ObjectNotCallableException : public FalxRuntimeException {
public:
    explicit ObjectNotCallableException(const std::string& object, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("'" + object + "' is not callable", location) {}
};

