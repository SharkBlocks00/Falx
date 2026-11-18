//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxRuntimeException.h"

class CannotAccessPropertyException : public FalxRuntimeException {
    using FalxRuntimeException::FalxRuntimeException;
};

class CannotSetPropertyException : public FalxRuntimeException {
public:
    explicit CannotSetPropertyException(const std::string& object, const std::string& property, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("'" + object + "' has no settable property '" + property + "'", location) {}
};