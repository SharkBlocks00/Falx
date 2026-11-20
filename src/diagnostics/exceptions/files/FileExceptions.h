//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "IOException.h"

class NotADirectoryException : IOException {
public:
    explicit NotADirectoryException(const std::string& path, const std::optional<SourceLocation>& location = std::nullopt)
        : IOException("'" + path + "' is not a directory", location) {}
};

class NotAFileException : IOException {
public:
    explicit NotAFileException(const std::string& path, const std::optional<SourceLocation>& location = std::nullopt)
        : IOException("'" + path + "' is not a file", location) {}
};

class PermissionDeniedException : IOException {
public:
    explicit PermissionDeniedException(const std::string& path, const std::optional<SourceLocation>& location = std::nullopt)
        : IOException("You do not have permission to work on '" + path + "'") {}
};
