//
// Created by SharkBlocks00 on 10/22/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../../Expression.h"
#include "../../../runtime/objects/advanced/FalxTuple.h"
#include <sstream>

class TupleLiteral : public Expression {
private:
    std::vector<std::shared_ptr<Expression>> m_elements;
public:
    TupleLiteral(const SourceLocation& location, std::vector<std::shared_ptr<Expression>>& elements)
        : Expression(location), m_elements(std::move(elements)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        std::vector<std::shared_ptr<FalxValue>> values;

        for (const auto& expr : m_elements) {
            values.push_back(expr->evaluate(interpreter, environment));
        }

        return std::make_unique<FalxTuple>(FalxTuple(values));
    }

    void print(std::ostream &os) const override {
        if (this->m_elements.empty()) {
            os << "(,,)";
        } if (this->m_elements.size() == 1) {
            os << "(";
            this->m_elements.front()->print(os);
            os << ",)";
        } else {
            std::ostringstream result;
            result << "(";

            for (std::size_t i = 0; i < this->m_elements.size(); ++i) {
                if (i > 0) {
                    result << ", ";
                }


            }
        }
    }
};
