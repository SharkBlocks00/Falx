//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Expression.h"
#include "../../runtime/objects/FalxBoolean.h"
#include "../../tokens/Token.h"

class BinaryExpression : public Expression {
private:
    std::shared_ptr<Expression> m_left;
    Token m_operator;
    std::shared_ptr<Expression> m_right;
public:
    BinaryExpression(const SourceLocation& location, std::shared_ptr<Expression>& left, Token  _operator, std::shared_ptr<Expression>& right)
        : Expression(location), m_left(std::move(left)),  m_operator(std::move(_operator)), m_right(std::move(right)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        const std::shared_ptr<FalxValue> leftValue{this->m_left->evaluate(interpreter, environment)}, rightValue{this->m_right->evaluate(interpreter, environment)};

        return applyOperator(this->m_operator.getTokenKind(), leftValue, rightValue);
    }

    [[nodiscard]] static std::shared_ptr<FalxValue> applyOperator(const TokenKind& operatorKind, const std::shared_ptr<FalxValue>& leftValue, const std::shared_ptr<FalxValue>& rightValue) {
        switch (operatorKind) {
            case TokenKind::PLUS: return leftValue->add(rightValue);
            case TokenKind::MINUS: return leftValue->minus(rightValue);
            case TokenKind::STAR: return leftValue->multiply(rightValue);
            case TokenKind::SLASH: return leftValue->divide(rightValue);
            case TokenKind::PERCENT: return leftValue->modulo(rightValue);

            // Assignment operators
            case TokenKind::GREATER:
                return std::make_unique<FalxValue>(FalxBoolean(leftValue->compareTo(rightValue) > 0));
            case TokenKind::GREATER_EQUAL:
                return std::make_unique<FalxValue>(FalxBoolean(leftValue->compareTo(rightValue) >= 0));
            case TokenKind::LESS:
                return std::make_unique<FalxValue>(FalxBoolean(leftValue->compareTo(rightValue) < 0));
            case TokenKind::LESS_EQUAL:
                return std::make_unique<FalxValue>(FalxBoolean(leftValue->compareTo(rightValue) <= 0));
            case TokenKind::EQUAL_EQUAL:
                return std::make_unique<FalxValue>(FalxBoolean(leftValue->equalsValue(rightValue)));
            case TokenKind::BANG_EQUAL:
                return std::make_unique<FalxValue>(FalxBoolean(!leftValue->equalsValue(rightValue)));

            default:
                throw std::runtime_error{"Cannot evaluate '" + leftValue->asString() + "' and '" + rightValue->asString() + "'"};
        }
    }
};
