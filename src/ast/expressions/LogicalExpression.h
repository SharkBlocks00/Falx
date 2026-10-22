//
// Created by SharkBlocks00 on 10/21/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Expression.h"
#include "../../tokens/Token.h"

class LogicalExpression : public Expression {
private:
    std::shared_ptr<Expression> m_left;
    Token m_operator;
    std::shared_ptr<Expression> m_right;
public:
    LogicalExpression(const SourceLocation& location, std::shared_ptr<Expression>& left, Token  _operator, std::shared_ptr<Expression>& right)
        : Expression(location), m_left(std::move(left)), m_operator(std::move(_operator)), m_right(std::move(right)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        std::shared_ptr<FalxValue> leftValue = this->m_left->evaluate(interpreter, environment);

        if (this->m_operator.getTokenKind() == TokenKind::OR && isTruthy(leftValue)) {
            return leftValue;
        }
        if (this->m_operator.getTokenKind() == TokenKind::AND && !isTruthy(leftValue)) {
            return leftValue;
        }

        return this->m_right->evaluate(interpreter, environment);
    }

    [[nodiscard]] static bool isTruthy(const std::shared_ptr<FalxValue>& value) {
        return value->asBool();
    }

    void print(std::ostream &os) const override {
        if (m_left) {
            m_left->print(os);
        } else {
            os << "(null)";
        }

        os << m_operator.asString();

        if (m_right) {
            m_right->print(os);
        } else {
            os << "(null)";
        }

        os << ';';
    }
};
