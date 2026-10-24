//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Expression.h"
#include "../../runtime/objects/FalxNull.h"

class IndexExpression : public Expression {
private:
    SourceLocation m_location;
    std::shared_ptr<Expression> m_index;
    std::shared_ptr<Expression> m_object;
    bool m_optional;
public:
    explicit IndexExpression(const SourceLocation& location, std::shared_ptr<Expression> index, std::shared_ptr<Expression> object, const bool optional = false)
        : Expression(location), m_location(location), m_index(std::move(index)), m_object(std::move(object)), m_optional(optional) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        const std::shared_ptr<FalxValue> object = this->m_object->evaluate(interpreter, environment);
        if (this->m_optional && typeid(*object) == typeid(FalxNull)) {
            throw std::runtime_error("NullShortCircuit");
        }
        const std::shared_ptr<FalxValue> index = this->m_index->evaluate(interpreter, environment);

        return object->index(index);
}
};
