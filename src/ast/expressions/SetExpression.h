//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "BinaryExpression.h"
#include "../Expression.h"
#include "../../tokens/Token.h"

class SetExpression : public Expression {
private:
    std::shared_ptr<Expression> m_object;
    std::string m_property;
    std::shared_ptr<Expression> m_value;
    Token* m_operator;
public:
    explicit SetExpression(const SourceLocation& location, std::shared_ptr<Expression> object, std::string property, std::shared_ptr<Expression> value, Token* _op = nullptr)
        : Expression(location), m_object(std::move(object)), m_property(std::move(property)), m_value(std::move(value)), m_operator(_op) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        const std::shared_ptr<FalxValue> object = this->m_object->evaluate(interpreter, environment);

        std::shared_ptr<FalxValue> evaluatedValue;

        if (this->m_operator != nullptr) {
            const std::shared_ptr<FalxValue> currentValue = object->get(this->m_property);
            const std::shared_ptr<FalxValue> rhsValue = this->m_value->evaluate(interpreter, environment);
            evaluatedValue = BinaryExpression::applyOperator(this->m_operator->getTokenKind(), currentValue, rhsValue);
        } else {
            evaluatedValue = this->m_value->evaluate(interpreter, environment);
        }

        object->set(this->m_property, evaluatedValue);
        return evaluatedValue;
    }
};
