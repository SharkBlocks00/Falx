//
// Created by SharkBlocks00 on 10/28/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <memory>
#include <string>
#include <utility>

#include "../../ast/Expression.h"
#include "../../ast/types/TypeExpression.h"

class FieldDefinition {
private:
    std::string m_name;
    std::shared_ptr<Expression> m_defaultValue;
    TypeExpressionPtr m_typeAnnotation;
    SourceLocation m_location;

public:
    explicit FieldDefinition(std::string  name, const std::shared_ptr<Expression>& defaultValue, TypeExpressionPtr  typeAnnotation, SourceLocation  location)
        : m_name(std::move(name)), m_defaultValue(defaultValue), m_typeAnnotation(std::move(typeAnnotation)), m_location(std::move(location)) {}

    std::shared_ptr<Expression> const& getDefaultValue() const { return m_defaultValue; }
    std::string const& getName() const { return m_name; }

    bool operator==(const FieldDefinition& other) const {
        return m_name == other.m_name && m_defaultValue == other.m_defaultValue && m_typeAnnotation == other.m_typeAnnotation;
    }
};
