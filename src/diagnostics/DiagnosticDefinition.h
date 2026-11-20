//
// Created by SharkBlocks00 on 11/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include <string>
#include <vector>

#include "DiagnosticSeverity.h"
#include "code/DiagnosticCode.hpp"

struct DiagnosticDefinition final {
    DiagnosticCode code;
    DiagnosticSeverity severity;
    std::string title;
    std::vector<std::string> help;
    std::vector<std::string> notes;
};
