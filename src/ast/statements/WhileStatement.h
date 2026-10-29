//
// Created by SharkBlocks00 on 10/29/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Expression.h"
#include "../Statement.h"
#include "../../exceptions/ContinueException.h"
#include "../../exceptions/BreakException.h"

class WhileStatement : public Statement {
private:
    std::shared_ptr<Expression> m_condition;
    std::vector<std::shared_ptr<Statement>> m_body;

public:
    explicit WhileStatement(const SourceLocation& location, std::shared_ptr<Expression> condition, std::vector<std::shared_ptr<Statement>> body)
        : Statement(location), m_condition(std::move(condition)), m_body(std::move(body)) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        while (this->m_condition->evaluate(interpreter, environment)->asBool()) {
            auto localEnv = Environment(environment);
            try {
                for (const auto& stmt : this->m_body) {
                    stmt->execute(interpreter, environment);
                }
            } catch (ContinueException&) {
                continue;
            } catch (BreakException&) {
                break;
            }
        }
    }

    void print(std::ostream &os) const override {
        os << "WhileStatement";
    }
};
