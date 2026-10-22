//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "SourceLocation.h"
#include "../runtime/environment/Environment.h"
#include "../runtime/interpreter/Interpreter.h"

class Statement {
private:
    SourceLocation m_location;
public:
    virtual ~Statement() = default;
    explicit Statement(SourceLocation location) : m_location(std::move(location)) {}

    [[nodiscard]] SourceLocation getSourceLocation() const { return m_location; }
    virtual void execute(const Interpreter& interpreter, Environment& environment)=0;
};
