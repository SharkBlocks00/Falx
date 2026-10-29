//
// Created by SharkBlocks00 on 10/26/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "TupleTypeExpression.h"

#include <stdexcept>
#include <sstream>
#include <utility>

TupleTypeExpression::TupleTypeExpression(const SourceLocation &location, std::vector<TypeExpressionPtr> elements)
    : TypeExpression(location), m_elements(std::move(elements)) {
    for (const auto& element : m_elements) {
        if (!element) {
            throw std::invalid_argument("TupleTypeExpression cannot contain null elements");
        }
    }
}

const std::vector<TypeExpressionPtr> &TupleTypeExpression::getElements() const {
    return m_elements;
}

std::string TupleTypeExpression::toString() const {
    std::ostringstream result;
    result << '(';

    for (std::size_t i = 0; i < m_elements.size(); ++i) {
        if (i > 0) {
            result << ", ";
        }
        result << m_elements[i]->toString();
    }

    result << ")";
    return result.str();
}
