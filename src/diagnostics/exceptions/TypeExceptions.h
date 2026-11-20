//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxException.h"

class FalxTypeException : public FalxException {
    using FalxException::FalxException;
};


class InvalidTypeAnnotationException : public FalxTypeException {
    using FalxTypeException::FalxTypeException;
};

class MissingReturnException : public FalxTypeException {
public:
    explicit MissingReturnException(const std::string& function, const std::string& returnType, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxTypeException("Not all code paths in function '" + function + "' return a value of type '" + returnType + "'", location) {}
};

class NullableValueException : public FalxTypeException {
    using FalxTypeException::FalxTypeException;
};

class TypeMismatchException : public FalxTypeException {
    using FalxTypeException::FalxTypeException;
};

class UnknownTypeException : public FalxTypeException {
public:
    explicit UnknownTypeException(const std::string& type, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxTypeException("Unknown type '" + type + "'", location) {}
};

class VoidValueException : public FalxTypeException {
    using FalxTypeException::FalxTypeException;
};

class TypeCheckError : public FalxTypeException {
    using FalxTypeException::FalxTypeException;
};