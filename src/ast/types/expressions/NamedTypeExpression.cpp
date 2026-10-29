//
// Created by SharkBlocks00 on 10/25/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "NamedTypeExpression.h"

#include<sstream>
#include <utility>

NamedTypeExpression::NamedTypeExpression(const SourceLocation &location, std::string name, std::vector<TypeExpressionPtr> arguments)
    : TypeExpression(location), m_name(std::move(name)), m_arguments(std::move(arguments)) {}

const std::string &NamedTypeExpression::getName() const {
    return m_name;
}

const std::vector<TypeExpressionPtr> &NamedTypeExpression::getArguments() const {
    return m_arguments;
}

std::string NamedTypeExpression::toString() const {
    if (m_arguments.empty()) {
        return m_name;
    }

    std::ostringstream result;
    result << m_name << '<';

    for (std::size_t i = 0; i < m_arguments.size(); ++i) {
        if (i > 0) {
            result << ", ";
        }
        result << m_arguments[i]->toString();
    }

    result << ">";
    return result.str();
}
