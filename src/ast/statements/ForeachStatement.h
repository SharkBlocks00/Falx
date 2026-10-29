//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Expression.h"
#include "../Statement.h"
#include "../../exceptions/BreakException.h"
#include "../../exceptions/ContinueException.h"
#include "../../runtime/objects/FalxIterable.h"

class ForeachStatement : public Statement {
private:
    std::string m_variable;
    std::shared_ptr<Expression> m_iterable;
    std::vector<std::shared_ptr<Statement>> m_body;
public:
    explicit ForeachStatement(const SourceLocation& location, const std::string &variable, const std::shared_ptr<Expression> &iterable, const std::vector<std::shared_ptr<Statement>> &body)
        : Statement(location), m_variable(variable), m_iterable(iterable), m_body(body) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        std::shared_ptr<FalxValue> value = this->m_iterable->evaluate(interpreter, environment);

        auto iterable = std::dynamic_pointer_cast<FalxIterable>(value);

        if (!iterable) {
            throw std::runtime_error("Cannot iterate over value!");
        }

        for (const auto& element : iterable->iterate()) {
            Environment loopEnv = Environment(environment);
            loopEnv.define(this->m_variable, element, false);

            try {
                for (const auto& stmt : this->m_body) {
                    stmt->execute(interpreter, environment);
                }
            } catch (ContinueException) {
            } catch (BreakException) {
                break;
            }
        }
    }
};
