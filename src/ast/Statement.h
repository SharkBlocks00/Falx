//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "SourceLocation.h"
#include "../runtime/environment/Environment.h"

class Interpreter;

class Statement {
private:
    SourceLocation m_location;
public:
    virtual ~Statement() = default;
    explicit Statement(SourceLocation location) : m_location(std::move(location)) {}

    [[nodiscard]] SourceLocation getSourceLocation() const { return m_location; }
    virtual void execute(const Interpreter& interpreter, Environment& environment)=0;
    virtual void print(std::ostream& os) const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Statement& statement) {
    statement.print(os);
    return os;
}

