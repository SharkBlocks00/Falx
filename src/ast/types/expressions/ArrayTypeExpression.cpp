//
// Created by SharkBlocks00 on 10/25/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "ArrayTypeExpression.h"

#include <stdexcept>
#include <utility>

ArrayTypeExpression::ArrayTypeExpression(const SourceLocation &location, TypeExpressionPtr element)
    : TypeExpression(location), m_element(std::move(element)) {
    if (!m_element) {
        throw std::invalid_argument("ArrayTypeExpression requires an element expression");
    }
}

const TypeExpressionPtr& ArrayTypeExpression::getElement() const {
    return m_element;
}

std::string ArrayTypeExpression::toString() const {
    const std::string element = m_element->toString();

    if (m_element->needsParenthesisAsPostfix()) {
        return "(" + element + ")[]";
    }

    return element + "[]";
}
