//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxException.h"

class FalxCLIException : public FalxException {
    using FalxException::FalxException;
};

class InvalidFileTypeException : public FalxException {
public:
    explicit InvalidFileTypeException(const std::string& flag)
        : FalxException("Invalid filetype after '" + flag + "'") {}
};

class MissingFilenameException : public FalxException {
public:
    explicit MissingFilenameException(const std::string& flag)
        : FalxException("Missing filename after '" + flag + "'") {}
};
