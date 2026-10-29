//
// Created by SharkBlocks00 on 10/28/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <sstream>
#include <utility>

#include "../Statement.h"
#include "../Expression.h"
#include "../../runtime/objects/FalxBoolean.h"

class IfStatement : public Statement {
private:
    std::shared_ptr<Expression> m_condition;
    std::vector<std::shared_ptr<Statement>> m_statements;
    std::shared_ptr<Statement> m_elseBody;

public:
    explicit IfStatement(const SourceLocation& location, std::shared_ptr<Expression> condition,
        std::vector<std::shared_ptr<Statement>> statements, std::shared_ptr<Statement> elseBody)
            : Statement(location), m_condition(std::move(condition)), m_statements(std::move(statements)), m_elseBody(std::move(elseBody)) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        if (isTruthy(this->m_condition->evaluate(interpreter, environment))) {
            auto scope = Environment(environment);
            for (const auto& stmt : this->m_statements) {
                stmt->execute(interpreter, environment);
            }
        } else if (this->m_elseBody) {
            this->m_elseBody->execute(interpreter, environment);
        }
    }

    [[nodiscard]] static bool isTruthy(const std::shared_ptr<FalxValue>& value) {
        if (typeid(value) == typeid(FalxBoolean)) {
            return value->asBool();
        }
        return false;
    }

    void print(std::ostream &os) const override {
        std::ostringstream result;
        result << "IfStatement(";
        m_condition->print(os);
        result << ");";
    }
};
