//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxRuntimeException.h"

class ImmutableValueException : public FalxRuntimeException {
public:
    explicit ImmutableValueException(const std::string& type, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot modify an immutable " + type, location) {}
};

class UnexpectedTypeException : public FalxRuntimeException {
public:
    explicit UnexpectedTypeException(const std::string& expected, const std::string& given, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Expected " + expected + ", but got '" + given + "'", location) {}
};