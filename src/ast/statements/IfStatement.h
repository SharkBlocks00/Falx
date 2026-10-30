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
                stmt->execute(interpreter, scope);
            }
        } else if (this->m_elseBody) {
            this->m_elseBody->execute(interpreter, environment);
        }
    }

    [[nodiscard]] static bool isTruthy(const std::shared_ptr<FalxValue>& value) {
        if (!value) {
            throw std::runtime_error("If condition evaluated to nullptr");
        }
        if (const auto* b = dynamic_cast<const FalxBoolean*>(value.get())) {
            return b->asBool();
        }
        throw std::runtime_error("If condition must evaluate to a boolean");
    }

    void print(std::ostream &os) const override {
        os << "IfStatement(";
        m_condition->print(os);
        os << ")";
    }
};
