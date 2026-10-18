//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//


#pragma once
#include <string>


class SourceLocation {
private:
    std::string m_filename;
    std::size_t m_line;
    std::size_t m_column;
public:
    SourceLocation(std::string filename, std::size_t line, std::size_t column);

    [[nodiscard]] std::string asString() const;
    [[nodiscard]] std::string getFilename() const;
    [[nodiscard]] std::size_t getLine() const;
    [[nodiscard]] std::size_t getColumn() const;
};
