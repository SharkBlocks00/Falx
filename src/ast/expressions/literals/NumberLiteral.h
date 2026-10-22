//
// Created by SharkBlocks00 on 10/19/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../../Expression.h"
#include "../../../runtime/objects/advanced/FalxNumber.h"
#include "../../../tokens/Token.h"

class NumberLiteral : public Expression {
private:
    Number m_value;
public:
    explicit NumberLiteral(const Token& token)
        : Expression(token.getLocation()) {
        if (const std::string lexeme = token.getLexeme(); lexeme.contains(".")) {
            this->m_value = std::stod(lexeme);
        } else {
            this->m_value = std::stoi(lexeme);
        }
    }

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter,  Environment &environment) const override {
        return std::make_unique<FalxValue>(FalxNumber(m_value));
    }
};
