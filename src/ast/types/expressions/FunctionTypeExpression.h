//
// Created by SharkBlocks00 on 10/26/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <vector>

#include "../TypeExpression.h"

class FunctionTypeExpression final : public TypeExpression {
private:
    std::vector<TypeExpressionPtr> m_parameters;
    TypeExpressionPtr m_returnType;

public:
    FunctionTypeExpression(const SourceLocation& location, std::vector<TypeExpressionPtr> parameters, TypeExpressionPtr returnType);

    [[nodiscard]] const std::vector<TypeExpressionPtr>& getParameters() const;
    [[nodiscard]] const TypeExpressionPtr& getReturnType() const;

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool needsParenthesisAsPostfix() const override { return true; }
};

