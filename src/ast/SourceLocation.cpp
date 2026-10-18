//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "SourceLocation.h"


SourceLocation::SourceLocation(std::string filename, const std::size_t line, const std::size_t column)
    : m_filename{std::move(filename)}, m_line{line}, m_column{column} {}

[[nodiscard]] std::string SourceLocation::asString() const {
    return this->m_filename + ":" + std::to_string(this->m_line) + ":" + std::to_string(this->m_column);
}

[[nodiscard]] std::string SourceLocation::getFilename() const {
    return this->m_filename;
}

[[nodiscard]] std::size_t SourceLocation::getLine() const {
    return this->m_line;
}

[[nodiscard]] std::size_t SourceLocation::getColumn() const {
    return this->m_column;
}