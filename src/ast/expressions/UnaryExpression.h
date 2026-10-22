//
// Created by SharkBlocks00 on 10/21/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Expression.h"
#include "../../runtime/objects/FalxBoolean.h"
#include "../../runtime/objects/advanced/FalxNumber.h"
#include "../../tokens/Token.h"

inline std::variant<int, double> operator-(const std::variant<int, double>& other) {
    if (std::holds_alternative<int>(other)) {
        return -std::get<int>(other);
    } return -std::get<double>(other);
}

class UnaryExpression : public Expression {
private:
    Token m_operator;
    std::shared_ptr<Expression> m_expression;
public:
    explicit UnaryExpression(SourceLocation location, Token op, std::shared_ptr<Expression> expression)
        : Expression(std::move(location)), m_operator(std::move(op)), m_expression(std::move(expression)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        std::shared_ptr<FalxValue> value = this->m_expression->evaluate(interpreter, environment);

        switch (this->m_operator.getTokenKind()) {
            case TokenKind::MINUS: return std::make_shared<FalxValue>(FalxNumber(-value->asNumber()));
            case TokenKind::BANG: return std::make_shared<FalxValue>(FalxBoolean(!value->asBool()));
            default: return value;
        }

    }

};
