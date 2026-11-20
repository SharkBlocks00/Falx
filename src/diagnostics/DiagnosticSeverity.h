//
// Created by SharkBlocks00 on 11/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <string>

enum class DiagnosticSeverity {
    ERROR,
    WARNING,
    NOTE
};

[[nodiscard]] inline std::string toString(const DiagnosticSeverity severity) {
    switch (severity) {
        case DiagnosticSeverity::ERROR: return "error";
        case DiagnosticSeverity::WARNING: return "warning";
        case DiagnosticSeverity::NOTE: return "note";

        default: return "UNKNOWN";
    }
}