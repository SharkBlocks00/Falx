//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../../Expression.h"
#include "../../../runtime/objects/advanced/FalxMap.h"
#include "../../../runtime/objects/advanced/FalxString.h"

class MapLiteral : public Expression {
private:
    std::map<std::string, std::shared_ptr<Expression>> m_values{};
public:
    explicit MapLiteral(const SourceLocation& location, const std::map<std::string, std::shared_ptr<Expression>>& values)
        : Expression(location) , m_values(values) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter& interpreter, Environment& environment) const override {
        std::map<std::shared_ptr<FalxValue>, std::shared_ptr<FalxValue>> elements{};

        for (const auto& [key, expr] : this->m_values) {
            const std::shared_ptr<FalxValue>& value = expr->evaluate(interpreter, environment);
            elements[std::make_unique<FalxValue>(FalxString{key})] = value;
        }

        return std::make_unique<FalxValue>(FalxMap{elements});
    }

    void print(std::ostream &os) const override {
        os << "MapLiteral(";
        for (const auto& [key, expr] : this->m_values) {
            os << key << " : ";

            if (expr) {
                expr->print(os);
            } else {
                os << "(null)";
            }
        }

        os << ")";
    }
};
