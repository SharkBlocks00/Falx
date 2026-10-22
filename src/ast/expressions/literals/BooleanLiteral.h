//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../../Expression.h"
#include "../../../runtime/objects/FalxBoolean.h"
#include "../../../tokens/Token.h"

class BooleanLiteral : public Expression {
private:
    bool m_value;
public:
    explicit BooleanLiteral(const Token& token) : Expression(token.getLocation()) {
        const Literal literal{token.getLiteral()};

        if (const std::string val{to_string(literal)}; val == "true" || val == "false") {
            m_value = (val == "true");
        }
    }

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter& interpreter, Environment& environment) const override {
        return std::make_unique<FalxBoolean>(FalxBoolean(this->m_value));
    }

    void print(std::ostream &os) const override {
        os << "BooleanLiteral(" << this->m_value << ")";
    }
};
