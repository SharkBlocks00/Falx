//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Expression.h"
#include "../Statement.h"

class ExpressionStatement : public Statement {
private:
    std::shared_ptr<Expression> m_expression;
public:
    explicit ExpressionStatement(const SourceLocation& location, const std::shared_ptr<Expression>& expression)
        : Statement(location), m_expression(expression) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        static_cast<void>(this->m_expression->evaluate(interpreter, environment));
    }

    void print(std::ostream& os) const override {
        if (m_expression) {
            m_expression->print(os);
        }

        os << ';';
    }
};
