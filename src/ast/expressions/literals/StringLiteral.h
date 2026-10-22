//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../../Expression.h"
#include "../../../runtime/objects/advanced/FalxString.h"
#include "../../../tokens/Token.h"

class StringLiteral : public Expression {
private:
    std::string m_value;
public:
    explicit StringLiteral(const Token& token)
        : Expression(token.getLocation()), m_value(std::move(to_string(token.getLiteral()))) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter& interpreter, Environment& environment) const override {
        return std::make_unique<FalxValue>(FalxString(this->m_value));
    }

};
