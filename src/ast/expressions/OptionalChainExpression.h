//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Expression.h"
#include "../../runtime/objects/FalxNull.h"

class OptionalChainExpression : public Expression {
private:
    std::shared_ptr<Expression> m_chain;
public:
    explicit OptionalChainExpression(const SourceLocation& location, std::shared_ptr<Expression> chain)
        : Expression(location), m_chain(std::move(chain)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        try {
            return this->m_chain->evaluate(interpreter, environment);
        } catch (std::runtime_error &e) {
            return std::make_unique<FalxNull>(FalxNull());
        }
    }
};
