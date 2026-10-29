//
// Created by SharkBlocks00 on 10/25/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../TypeExpression.h"
#include <vector>

class UnionTypeExpression final : public TypeExpression {
private:
    std::vector<TypeExpressionPtr> m_members;

public:
    UnionTypeExpression(const SourceLocation& location, std::vector<TypeExpressionPtr> members);

    [[nodiscard]] const std::vector<TypeExpressionPtr>& getMembers() const;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool needsParenthesisAsPostfix() const override { return true; }
};
