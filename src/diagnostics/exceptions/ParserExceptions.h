//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxException.h"

class FalxParserException : public FalxException {
    using FalxException::FalxException;
};

class BreakOutsideLoopException : public FalxParserException {
public:
    explicit BreakOutsideLoopException(const std::optional<SourceLocation> &location = std::nullopt)
        : FalxParserException("'break' called outside a valid loop", location) {}
};

class ContinueOutsideLoopException : public FalxParserException {
    explicit ContinueOutsideLoopException(const std::optional<SourceLocation> &location = std::nullopt)
        : FalxParserException("'continue' called outside a valid loop", location) {}
};

class ReturnOutsideFunctionException : public FalxParserException {
    explicit ReturnOutsideFunctionException(const std::optional<SourceLocation> &location = std::nullopt)
        : FalxParserException("'return' called outside a valid function", location) {}
};

class InvalidAssignmentTargetException : public FalxParserException {
    explicit InvalidAssignmentTargetException(const std::string &target, const std::optional<SourceLocation> &location = std::nullopt)
        : FalxParserException(target, location) {}
};

class InvalidDefaultValueCreationException : public FalxParserException {
    explicit InvalidDefaultValueCreationException(const std::string& value, const std::optional<SourceLocation>& location)
        : FalxParserException("Cannot have non default parameter " + value + "after default parameters.", location) {}
};

class UnexpectedEndOfInputException : public FalxParserException {
    using FalxParserException::FalxParserException;
};

class UnexpectedTokenException : public FalxParserException {
    using FalxParserException::FalxParserException;
};