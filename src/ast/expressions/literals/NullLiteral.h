//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../../Expression.h"
#include "../../../runtime/objects/FalxNull.h"
#include "../../../tokens/Token.h"

class NullLiteral : public Expression {
private:
    Literal m_literal;
public:
    explicit NullLiteral(const Token& token) : Expression(token.getLocation()), m_literal(token.getLiteral()) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter& interpreter, Environment& environment) const override {
        return std::make_unique<FalxValue>(FalxNull{});
    }
};
