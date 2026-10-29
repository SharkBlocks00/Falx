//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <memory>
#include <utility>

#include "../SourceLocation.h"


class TypeExpression {
private:
    SourceLocation m_location;
public:
    explicit TypeExpression(SourceLocation   location)
        : m_location(std::move(location)) {}

    virtual ~TypeExpression() = default;

    [[nodiscard]] SourceLocation getLocation() const { return m_location; }
    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual bool needsParenthesisAsPostfix() const { return false; }
};

using TypeExpressionPtr = std::shared_ptr<const TypeExpression>;
