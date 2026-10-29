//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <set>

#include "../Statement.h"
#include "../expressions/DestructureAssignmentExpression.h"
#include "../expressions/VariableExpression.h"

class DestructureDeclarationStatement : public Statement {
private:
    SourceLocation m_location;
    std::vector<VariableExpression> m_targets;
    std::shared_ptr<Expression> m_initializer;
    bool m_mutable;
    bool m_exported;
public:
    explicit DestructureDeclarationStatement(const SourceLocation& location, std::vector<VariableExpression> targets, const std::shared_ptr<Expression> &initializer, const bool _mut, const bool _exported)
        : Statement(location), m_location(location), m_targets(std::move(targets)), m_initializer(initializer), m_mutable(_mut), m_exported(_exported) {}

    void execute(const Interpreter &interpreter, Environment &environment, Environment* export_environment = nullptr) const {
        const std::shared_ptr<FalxValue> value = this->m_initializer->evaluate(interpreter, environment);
        const auto elements = DestructureAssignmentExpression::unpack(value, this->m_targets.size(), this->m_location);

        std::set<std::string> seen{};
        for (const auto& target : this->m_targets) {
            if (seen.contains(target.getName()) || environment.getVariables().contains(target.getName())) {
                throw std::runtime_error("Variable already declared");
            }
            seen.insert(target.getName());
        }

        for (std::size_t i = 0; i < this->m_targets.size() && i < elements.size(); ++i) {
            environment.define(m_targets[i].getName(), elements[i], this->m_mutable);
            if (export_environment && this->m_exported) {
                export_environment->define(m_targets[i].getName(), elements[i], this->m_mutable);
            }
        }
    }
};
