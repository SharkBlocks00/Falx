//
// Created by SharkBlocks00 on 11/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <optional>
#include <vector>

#include "DiagnosticSeverity.h"
#include "../ast/SourceLocation.h"
#include "code/DiagnosticCode.hpp"

struct Diagnostic {
    DiagnosticCode code;
    DiagnosticSeverity severity;
    std::string message;
    std::optional<SourceLocation> location{std::nullopt};

    std::vector<std::string> notes;
    std::vector<std::string> help;
};
