//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../FalxException.h"

class FalxRuntimeException : public FalxException {
    using FalxException::FalxException;
};


// Miscellaneous exceptions

class AssertionFailedException : public FalxRuntimeException {
public:
    explicit AssertionFailedException(const std::optional<std::string>& message = std::nullopt, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Assertion failed" + (message.has_value()? ": " + *message: ""), location) {}
};

class ExpectedValueException : public FalxRuntimeException {
    using FalxRuntimeException::FalxRuntimeException;
};

class ObjectNotIterableException : public FalxRuntimeException {
public:
    explicit ObjectNotIterableException(const std::string& object, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("'" + object + "' is not iterable", location) {}
};

class RecursionDepthExceededException : public FalxRuntimeException {
public:
    explicit RecursionDepthExceededException(const int maxDepth, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Maximum recursion depth of " + std::to_string(maxDepth) + " exceeded", location) {}
};