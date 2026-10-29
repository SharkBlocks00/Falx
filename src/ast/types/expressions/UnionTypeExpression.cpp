//
// Created by SharkBlocks00 on 10/25/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "UnionTypeExpression.h"

#include <stdexcept>
#include <sstream>
#include <utility>

UnionTypeExpression::UnionTypeExpression(const SourceLocation &location, std::vector<TypeExpressionPtr> members)
    : TypeExpression(location), m_members(std::move(members)) {
    if (m_members.size() < 2) {
        throw std::invalid_argument("UnionTypeExpression requires at least two members");
    }

    for (const auto& member : m_members) {
        if (!member) {
            throw std::invalid_argument("UnionTypeExpression cannot contain null members");
        }
    }
}

const std::vector<TypeExpressionPtr> &UnionTypeExpression::getMembers() const {
    return m_members;
}

std::string UnionTypeExpression::toString() const {
    std::ostringstream result;

    for (std::size_t i = 0; i < m_members.size(); ++i) {
        if (i > 0) {
            result << " | ";
        }

        result << m_members[i]->toString();
    }

    return result.str();
}
