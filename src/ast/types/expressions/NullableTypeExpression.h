//
// Created by SharkBlocks00 on 10/25/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../TypeExpression.h"

class NullableTypeExpression final : public TypeExpression {
private:
    TypeExpressionPtr m_inner;

public:
    NullableTypeExpression(const SourceLocation& location, TypeExpressionPtr inner);

    [[nodiscard]] const TypeExpressionPtr& getInner() const;
    [[nodiscard]] std::string toString() const override;
};
