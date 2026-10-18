//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>
#include <vector>

#include "../../Expression.h"
#include "../../../runtime/objects/advanced/FalxArray.h"

class ArrayLiteral : public Expression {
private:
    std::vector<Expression> m_elements{};
public:
    ArrayLiteral(const SourceLocation &location, std::vector<Expression> elements)
        : Expression(location), m_elements(std::move(elements)) {}

    [[nodiscard]] std::vector<Expression> getElements() const { return m_elements; }
    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter& interpreter, const Environment& environment) const override {
        std::vector<std::shared_ptr<FalxValue>> result{};

        for (const auto& expr : this->m_elements) {
            result.push_back(expr.evaluate(interpreter, environment));
        }

        return std::make_unique<FalxArray>(FalxArray(result));
    }
};
