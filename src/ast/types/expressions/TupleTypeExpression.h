//
// Created by SharkBlocks00 on 10/26/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../TypeExpression.h"
#include <vector>

class TupleTypeExpression final : public TypeExpression {
private:
    std::vector<TypeExpressionPtr> m_elements;

public:
    TupleTypeExpression(const SourceLocation& location, std::vector<TypeExpressionPtr> elements);

    [[nodiscard]] const std::vector<TypeExpressionPtr>& getElements() const;
    [[nodiscard]] std::string toString() const override;
};
