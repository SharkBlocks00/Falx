//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Expression.h"
#include "../../runtime/objects/FalxNull.h"

class GetExpression : public Expression {
private:
    SourceLocation m_location;
    std::shared_ptr<Expression> m_object;
    std::string m_property;
    bool m_optional;
public:
    explicit GetExpression(const SourceLocation& location, const std::shared_ptr<Expression> &object, std::string  property, const bool optional = false)
        : Expression(location), m_location(location), m_object(object), m_property(std::move(property)), m_optional(optional) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        const std::shared_ptr<FalxValue> value = this->m_object->evaluate(interpreter, environment);
        if (this->m_optional && typeid(value) == typeid(FalxNull)) {
            throw std::runtime_error{"NullShortCircuit"};
        }

        return value->get(this->m_property);
    }
};
