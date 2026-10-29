//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <set>
#include <utility>

#include "../Statement.h"
#include "../../runtime/values/ParameterDefinition.h"
#include "../../runtime/values/UserFunction.h"

class FunctionDeclaration : public Statement {
private:
    SourceLocation m_location;
    std::string m_name;
    std::vector<ParameterDefinition> m_parameters;
    std::vector<std::shared_ptr<Statement>> m_body;
    bool m_exported;
    TypeExpressionPtr m_returnType;
    std::unique_ptr<Environment> m_exportEnvironment{nullptr};

public:
    explicit FunctionDeclaration(const SourceLocation& location, std::string name,
        std::vector<ParameterDefinition> parameters, std::vector<std::shared_ptr<Statement>> body,
        const bool exported = false, TypeExpressionPtr returnType = nullptr)
            : Statement(location), m_location(location), m_name(std::move(name)), m_parameters(std::move(parameters)),
                m_body(std::move(body)), m_exported(exported), m_returnType(std::move(returnType)) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        std::set<std::string> seen{};

        for (const auto& parameter : m_parameters) {
            if (seen.contains(parameter.getName())) {
                throw std::runtime_error("Parameter already exists!");
            }

            seen.insert(parameter.getName());
        }

        try {
            const auto function = std::make_shared<UserFunction>(UserFunction(this->m_parameters, this->m_body, environment));
            environment.define(this->m_name, function, false);
            if (this->m_exportEnvironment && this->m_exported) {
                this->m_exportEnvironment->define(this->m_name, function, false);
            }
        } catch (std::runtime_error& e) {
            throw std::runtime_error(e.what());
        }
    }

    void setExportEnvironment(const Environment &environment) {
        m_exportEnvironment = std::make_unique<Environment>(environment);
    }

    void print(std::ostream &os) const override {
        os << "FunctionDeclarationStatement";
    }

    [[nodiscard]] std::string getName() const { return m_name; }
};
