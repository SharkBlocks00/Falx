//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <filesystem>
#include <vector>

#include "FalxRuntimeException.h"

class CircularModuleDependencyException : public FalxRuntimeException {
public:
    explicit CircularModuleDependencyException(const std::filesystem::path& path, const std::vector<std::filesystem::path>& modules)
        : FalxRuntimeException("Circular module dependency detected: " + buildDependencyChain(path, modules)) {}

private:
    static std::string buildDependencyChain(const std::filesystem::path& path, const std::vector<std::filesystem::path>& modules) {
        std::string dependencyChain;

        for (const auto& module : modules) {
            if (!dependencyChain.empty()) {
                dependencyChain += " -> ";
            }

            dependencyChain += module.stem().string();
        }

        if (!dependencyChain.empty()) {
            dependencyChain += " -> ";
        }

        dependencyChain += path.stem().string();
        return dependencyChain;
    }
};

class ExportMutableValueException : public FalxRuntimeException {
    using FalxRuntimeException::FalxRuntimeException;
};

class ModuleNotFoundException : public FalxRuntimeException {
public:
    explicit ModuleNotFoundException(const std::string& name, const std::string& path)
        : FalxRuntimeException("Cannot find module '" + name + "' in path '" + path + "'") {}
};