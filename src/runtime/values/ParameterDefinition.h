//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <memory>
#include <string>
#include <utility>

#include "../../ast/Expression.h"
#include "../../ast/types/TypeExpression.h"

class ParameterDefinition {
private:
    std::string m_name;
    std::shared_ptr<Expression> m_defaultValue;
    TypeExpressionPtr m_typeAnnotation;
public:
    explicit ParameterDefinition(std::string name, const std::shared_ptr<Expression> &defaultValue=nullptr, TypeExpressionPtr typeAnnotation=nullptr)
        : m_name(std::move(name)), m_defaultValue(defaultValue), m_typeAnnotation(std::move(typeAnnotation)) {}

    [[nodiscard]] std::string getName() const { return m_name; }
    [[nodiscard]] std::shared_ptr<Expression> getDefaultValue() const { return m_defaultValue; }
    [[nodiscard]] const TypeExpressionPtr& getTypeAnnotation() const { return m_typeAnnotation; }
};
