//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <ratio>
#include <utility>

#include "BinaryExpression.h"
#include "../Expression.h"
#include "../../tokens/Token.h"

class IndexSetExpression : public Expression {
private:
    SourceLocation m_location;
    std::shared_ptr<Expression> m_object;
    std::shared_ptr<Expression> m_index;
    std::shared_ptr<Expression> m_value;
    std::optional<Token> m_operator;
public:
    explicit IndexSetExpression(const SourceLocation& location, std::shared_ptr<Expression> object,std::shared_ptr<Expression> index, std::shared_ptr<Expression> value, std::optional<Token> _op = std::nullopt)
        : Expression(location), m_location(location), m_object(std::move(object)), m_index(std::move(index)), m_value(std::move(value)), m_operator(std::move(_op)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        const std::shared_ptr<FalxValue> object = this->m_object->evaluate(interpreter, environment);
        const std::shared_ptr<FalxValue> index = this->m_index->evaluate(interpreter, environment);
        std::shared_ptr<FalxValue> evaluatedValue;

        if (this->m_operator != std::nullopt) {
            const std::shared_ptr<FalxValue> currentValue = object->index(index);
            const std::shared_ptr<FalxValue> rhsValue = this->m_value->evaluate(interpreter, environment);
            evaluatedValue = BinaryExpression::applyOperator(this->m_operator->getTokenKind(), currentValue, rhsValue);
        } else {
            evaluatedValue = this->m_value->evaluate(interpreter, environment);
        }

        object->indexAssign(index, evaluatedValue);
        return evaluatedValue;
    }

    void print(std::ostream &os) const override {
        os << "IndexSetExpression";
    }
};
