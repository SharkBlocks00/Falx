//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//


#pragma once
#include <utility>

#include "SourceLocation.h"
#include "../runtime/environment/Environment.h"
#include "../runtime/interpreter/Interpreter.h"


class Expression {
private:
    SourceLocation m_location;
public:
    virtual ~Expression() = default;

    explicit Expression(SourceLocation location) : m_location(std::move(location)) {}

    [[nodiscard]] SourceLocation getSourceLocation() const { return m_location; }
    [[nodiscard]] virtual std::shared_ptr<FalxValue> evaluate(const Interpreter& interpreter, Environment& environment) const=0;
};
