//
// Created by SharkBlocks00 on 10/25/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "NullableTypeExpression.h"

#include <stdexcept>
#include <utility>

NullableTypeExpression::NullableTypeExpression(const SourceLocation &location, TypeExpressionPtr inner)
    : TypeExpression(location), m_inner(std::move(inner)) {
    if (!m_inner) {
        throw std::invalid_argument("NullableTypeExpression requires an inner expression");
    }
}

const TypeExpressionPtr& NullableTypeExpression::getInner() const {
    return m_inner;
}

std::string NullableTypeExpression::toString() const {
    const std::string inner = m_inner->toString();

    if (m_inner->needsParenthesisAsPostfix()) {
        return "(" + inner + ")?";
    }

    return inner + "?";
}
