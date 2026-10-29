//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../TypeExpression.h"

#include <string>
#include <vector>

class NamedTypeExpression final : public TypeExpression {
private:
    std::string m_name;
    std::vector<TypeExpressionPtr> m_arguments;

public:
    NamedTypeExpression(
        const SourceLocation& location,
        std::string name,
        std::vector<TypeExpressionPtr> arguments = {}
    );

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::vector<TypeExpressionPtr>& getArguments() const;

    [[nodiscard]] std::string toString() const override;
};

