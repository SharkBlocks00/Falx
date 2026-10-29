//
// Created by SharkBlocks00 on 10/26/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FunctionTypeExpression.h"

#include <stdexcept>
#include <sstream>
#include <utility>

FunctionTypeExpression::FunctionTypeExpression(const SourceLocation &location, std::vector<TypeExpressionPtr> parameters, TypeExpressionPtr returnType)
    : TypeExpression(location), m_parameters(std::move(parameters)), m_returnType(std::move(returnType)) {
    if (!m_returnType) {
        throw std::invalid_argument("FunctionTypeExpression requires a return type");
    }

    for (const auto& parameter : m_parameters) {
        if (!parameter) {
            throw std::invalid_argument("FunctionTypeExpression cannot contain null parameters");
        }
    }
}

const std::vector<TypeExpressionPtr> &FunctionTypeExpression::getParameters() const {
    return m_parameters;
}

const TypeExpressionPtr &FunctionTypeExpression::getReturnType() const {
    return m_returnType;
}

std::string FunctionTypeExpression::toString() const {
    std::ostringstream result;
    result << '(';

    for (std::size_t i = 0; i < m_parameters.size(); ++i) {
        if (i > 0) {
            result << ", ";
        }

        result << m_parameters[i]->toString();
    }

    result << ") -> " << m_returnType->toString();
    return result.str();
}

