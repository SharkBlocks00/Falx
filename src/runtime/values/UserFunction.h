//
// Created by SharkBlocks00 on 10/26/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "ParameterDefinition.h"
#include "../../exceptions/ReturnException.h"
#include "../../packages/Callable.h"
#include "../objects/FalxNull.h"

class UserFunction : public Callable {
private:
    std::vector<ParameterDefinition> m_parameters;
    std::vector<std::shared_ptr<Statement>> m_statements;
    Environment m_closure;

public:
    explicit UserFunction(std::vector<ParameterDefinition> parameters, std::vector<std::shared_ptr<Statement>> statements, Environment closure)
        : m_parameters(std::move(parameters)), m_statements(std::move(statements)), m_closure(std::move(closure)) {}

    [[nodiscard]] std::string getTypeName() const override { return "function"; }
    [[nodiscard]] bool isStrict() const override { return false; }
    [[nodiscard]] std::size_t arity() const override { return this->m_parameters.size(); }

    [[nodiscard]] std::shared_ptr<FalxValue> call(const Interpreter& interpreter, std::vector<std::shared_ptr<FalxValue>> &arguments) const override {
        if (arguments.size() > this->m_parameters.size()) {
            throw std::invalid_argument("Invalid argument count");
        }

        auto local = Environment(this->m_closure);

        for (std::size_t i = 0; i < this->m_parameters.size(); i++) {
            bool argExists{false};
            try {
                static_cast<void>(arguments.at(i));
                argExists = true;
            } catch (std::out_of_range &e) {
                argExists = false;
            }

            if (this->m_parameters[i].getDefaultValue() && !argExists) {
                local.define(this->m_parameters[i].getName(), this->m_parameters[i].getDefaultValue()->evaluate(interpreter, local), true);
            } else if (argExists) {
                local.define(this->m_parameters[i].getName(), arguments[i], true);
            } else {
                throw std::invalid_argument("Invalid argument");
            }
        }

       try {
           for (const auto& statement : this->m_statements) {
               statement->execute(interpreter, local);
           }
       } catch (ReturnException &r) {
           return r.getValue();
       }

        return std::make_shared<FalxNull>(FalxNull());
    }

    std::vector<ParameterDefinition> const& getParameters() const { return this->m_parameters; }
    std::vector<std::shared_ptr<Statement>> const& getStatements() const { return this->m_statements; }
};
