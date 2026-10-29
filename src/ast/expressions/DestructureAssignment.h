//
// Created by SharkBlocks00 on 10/22/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "VariableExpression.h"
#include "../Expression.h"
#include "../../runtime/objects/advanced/FalxTuple.h"

class DestructureAssignment : public Expression {
private:
    SourceLocation m_location;
    std::vector<VariableExpression> m_targets;
    std::shared_ptr<Expression> m_value;
public:
    explicit DestructureAssignment(const SourceLocation& location, const std::vector<VariableExpression>& targets, std::shared_ptr<Expression> values)
        : Expression(location), m_location(location), m_targets(std::move(targets)), m_value(std::move(values)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        const std::shared_ptr<FalxValue> value = this->m_value->evaluate(interpreter, environment);
        const std::vector<std::shared_ptr<FalxValue>> elements = unpack(value, this->m_targets.size(), m_location);

        for (const auto& target : this->m_targets) {
            environment.checkAssignable(target.getName());
        }

        for (std::size_t i = 0; i < m_targets.size() && i < elements.size(); ++i) {
            environment.assign(m_targets[i].getName(), elements[i]);
        }

        return value;
    }

    [[nodiscard]] static std::vector<std::shared_ptr<FalxValue>> unpack(std::shared_ptr<FalxValue> value, const int count, const SourceLocation& location) {
        if (typeid(*value) != typeid(FalxTuple)) {
            throw std::runtime_error("Expected tuple!");
        }
        const auto& tmp = static_cast<const FalxTuple&>(*value);
        if (tmp.getValues().size() != count) {
            throw std::runtime_error("Received invalid amount of `count`!");
        }

        return tmp.getValues();
    }

    void print(std::ostream &os) const override {

    }
};
