//
// Created by SharkBlocks00 on 11/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "Diagnostic.h"
#include "exceptions/FalxException.h"
#include "DiagnosticRegistry.hpp"

inline Diagnostic createDiagnostic(const FalxException& exception) {
    if (const DiagnosticDefinition* definition = DiagnosticRegistry::find(typeid(exception))) {
        return Diagnostic{definition->code, definition->severity, exception.getMessage(), exception.getLocation(), definition->notes, definition->help};
    }

    throw std::runtime_error{"No diagnostic definition registered for '" + std::string(typeid(exception).name()) + "'"};
}
