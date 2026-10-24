//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Expression.h"
#include "../../runtime/objects/FalxNull.h"

class NullCoalesceExpression : public Expression {
private:
    std::shared_ptr<Expression> m_left;
    std::shared_ptr<Expression> m_right;
public:
    explicit NullCoalesceExpression(const SourceLocation& location, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
        : Expression(location), m_left(std::move(left)), m_right(std::move(right)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        if (std::shared_ptr<FalxValue> value = this->m_left->evaluate(interpreter, environment); typeid(*value) != typeid(FalxNull)) {
            return value;
        }

        return this->m_right->evaluate(interpreter, environment);
    }
};
