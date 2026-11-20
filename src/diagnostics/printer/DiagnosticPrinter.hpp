//
// Created by SharkBlocks00 on 11/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Diagnostic.h"

#include <sstream>
#include <string>
#include <vector>

class DiagnosticPrinter {
private:
    [[nodiscard]] static std::vector<std::string> splitLines(const std::string& source) ;
public:
    static void print(const Diagnostic& diagnostic, const std::string& source);
};
