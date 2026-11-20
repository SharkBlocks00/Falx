//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "IOException.h"

class FileClosedException : IOException {
public:
    explicit FileClosedException(const std::string& path, const std::optional<SourceLocation>& location = std::nullopt)
        : IOException("File '" + path + "' has already been closed.", location) {}
};

class InvalidFileModeException : IOException {
public:
    explicit InvalidFileModeException(const std::string& mode, const std::optional<SourceLocation>& location = std::nullopt)
        : IOException("Invalid file opening mode '" + mode + "'", location) {}
};