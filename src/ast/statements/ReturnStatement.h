//
// Created by SharkBlocks00 on 10/28/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Expression.h"
#include "../Statement.h"
#include "../../exceptions/ReturnException.h"
#include "../../runtime/objects/FalxNull.h"

class ReturnStatement : public Statement {
private:
    std::shared_ptr<Expression> m_expression;

public:
    explicit ReturnStatement(const SourceLocation& location, const std::shared_ptr<Expression>& expression)
        : Statement(location), m_expression(expression) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        std::shared_ptr<FalxValue> object = std::make_shared<FalxNull>(FalxNull());
        if (this->m_expression != nullptr) {
            object = this->m_expression->evaluate(interpreter, environment);
        }

        throw ReturnException(object);
    }
};
