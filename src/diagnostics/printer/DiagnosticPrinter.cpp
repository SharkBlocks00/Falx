//
// Created by SharkBlocks00 on 11/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "DiagnosticPrinter.hpp"

#include <iomanip>
#include <iostream>

void DiagnosticPrinter::print(const Diagnostic &diagnostic, const std::string &source) {
    if (!diagnostic.location.has_value()) {
        std::cerr << toString(diagnostic.severity)
                  << "[" << toString(diagnostic.code) << "]: "
                  << diagnostic.message << '\n';
        return;
    }

    const auto& location = diagnostic.location.value();
    const auto lines = splitLines(source);

    const auto lineNumberWidth = std::to_string(lines.size()).size();

    std::cerr << location.getFilename() << ":"
              << location.getLine() << ":"
              << location.getColumn() << ": "
              << toString(diagnostic.severity)
              << "[" << toString(diagnostic.code) << "]: "
              << diagnostic.message << '\n';

    if (location.getLine() <= lines.size() && location.getLine() > 0) {
        const auto& errorLine = lines.at(location.getLine() - 1);

        std::cerr << "  "
                  << std::setw(lineNumberWidth)
                  << location.getLine()
                  << " | "
                  << errorLine << '\n';

        std::cerr << "  "
                  << std::string(lineNumberWidth, ' ')
                  << " | "
                  << std::string(location.getColumn() - 1, ' ')
                  << "^\n";
    }

    for (const auto& helpMessage : diagnostic.help) {
        std::cerr << "help: " << helpMessage << "\n\n";
    }

    std::cerr << '\n';

    for (const auto& note : diagnostic.notes) {
        std::cerr << "note: " << note << '\n';
    }
}

std::vector<std::string> DiagnosticPrinter::splitLines(const std::string &source) {
    std::vector<std::string> lines;
    std::istringstream stream(source);

    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    return lines;
}
