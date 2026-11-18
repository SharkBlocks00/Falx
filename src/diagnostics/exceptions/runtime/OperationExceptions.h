//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxRuntimeException.h"

class CannotAddWithException : public FalxRuntimeException {
public:
    explicit CannotAddWithException(const std::string& leftValue, const std::string& rightValue, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot add '" + leftValue + "' to '" + rightValue + "'", location) {}
};

class CannotCompareToException : public FalxRuntimeException {
public:
    explicit CannotCompareToException(const std::string& leftValue, const std::string& rightValue, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot compare '" + leftValue + "' with '" + rightValue + "'", location) {}
};

class CannotConvertToTypeException : public FalxRuntimeException {
public:
    explicit CannotConvertToTypeException(const std::string& name, const std::string& expectedType, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot convert '" + name + "' to " + expectedType, location) {}
};

class CannotDivideByValueException : public FalxRuntimeException {
public:
    explicit CannotDivideByValueException(const std::string& leftValue, const std::string& rightValue, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot divide '" + leftValue + "' by '" + rightValue + "'", location) {}
};

class CannotEvaluateValueException : public FalxRuntimeException {
public:
    explicit CannotEvaluateValueException(const std::string& name, const std::string& rightValue, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot evaluate '" + name + "' and '" + rightValue +"'", location) {}
};

class CannotInvertValueException : public FalxRuntimeException {
public:
    explicit CannotInvertValueException(const std::string& value, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot invert '" + value + "'", location) {}
};

class CannotMinusFromValueException : public FalxRuntimeException {
public:
    explicit CannotMinusFromValueException(const std::string& leftValue, const std::string& rightValue, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot minus '" + rightValue + "' from '" + leftValue + "'") {}
};

class CannotModWithValueException : public FalxRuntimeException {
public:
    explicit CannotModWithValueException(const std::string& leftValue, const std::string& rightValue, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot mod '" + leftValue + "' by '" +rightValue + "'") {}
};

class CannotMultiplyByValueException : public FalxRuntimeException {
public:
    explicit CannotMultiplyByValueException(const std::string& leftValue, const std::string& rightValue, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot multiply '" + leftValue + "' by '" + rightValue + "'") {}
};

class DivisionByZeroException : public FalxRuntimeException {
public:
    explicit DivisionByZeroException(const std::string& value, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot divide '" + value + "' by zero") {}
};