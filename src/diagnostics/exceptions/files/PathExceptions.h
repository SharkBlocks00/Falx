//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "IOException.h"

class FileAlreadyExistsException : IOException {
public:
    explicit FileAlreadyExistsException(const std::string& path, const std::optional<SourceLocation>& location)
        : IOException("File '" + path + "' already exists", location) {}
};

class FileNotFoundException : IOException {
public:
    explicit FileNotFoundException(const std::string& path, const std::optional<SourceLocation>& location)
        : IOException("File '" + path + "' does not exist", location) {}
};

class InvalidPathException : IOException {
public:
    explicit InvalidPathException(const std::string& path, const std::optional<SourceLocation>& location)
        : IOException("Path '" + path + "' is invalid", location) {}
};
