//
// Created by SharkBlocks00 on 10/25/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../TypeExpression.h"

class ArrayTypeExpression final : public TypeExpression {
private:
    TypeExpressionPtr m_element;

public:
    ArrayTypeExpression(const SourceLocation& location, TypeExpressionPtr element);

    [[nodiscard]] const TypeExpressionPtr& getElement() const;
    [[nodiscard]] std::string toString() const override;
};
