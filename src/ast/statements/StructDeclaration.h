//
// Created by SharkBlocks00 on 10/28/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "FunctionDeclaration.h"
#include "../Statement.h"
#include "../../runtime/values/FieldDefinition.h"
#include "../../runtime/values/StructConstructor.h"
#include "../../runtime/values/StructDefinition.h"

class StructDeclaration : public Statement {
private:
    std::string m_name;
    std::vector<FieldDefinition> m_fields;
    std::vector<FunctionDeclaration> m_methods;
    bool m_exported;
    std::unique_ptr<Environment> m_exportEnvironment{nullptr};

public:
    explicit StructDeclaration(const SourceLocation& location, std::string name, const std::vector<FieldDefinition>& fields, std::vector<FunctionDeclaration> methods, const bool exported)
        : Statement(location), m_name(std::move(name)), m_fields(fields), m_methods(std::move(methods)), m_exported(exported) {}

    void setExportEnvironment(std::unique_ptr<Environment>& environment) {
        m_exportEnvironment = std::move(environment);
    }

    void execute(const Interpreter &interpreter, Environment &environment) override {
        try {
            std::map<std::string, UserFunction> methods{};
            for (auto& method : m_methods) {
                method.execute(interpreter, environment);
                methods.insert_or_assign(method.getName(), static_cast<const UserFunction&>(*environment.get(method.getName())));
            }

            const StructDefinition definition{this->m_name, this->m_fields, methods, environment};
            const StructConstructor constructor{definition};

            environment.define(this->m_name, std::make_shared<FalxValue>(constructor), false);

            if (this->m_exportEnvironment && this->m_exported) {
                this->m_exportEnvironment->define(this->m_name, std::make_shared<FalxValue>(constructor), false);
            }
        } catch (std::runtime_error& e) {
            throw;
        }
    }

    void print(std::ostream &os) const override {
        os << "ReturnStatement";
    }
};
