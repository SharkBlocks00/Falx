//
// Created by SharkBlocks00 on 10/28/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../FalxValue.h"
#include "../../values/StructDefinition.h"

class FalxStruct : public FalxValue {
private:
    StructDefinition m_definition;
    Environment m_environment;
    std::map<std::string, std::shared_ptr<Callable>> m_methods;

public:
    explicit FalxStruct(const StructDefinition &definition)
        : m_definition(definition) {
        this->m_environment = Environment(definition.getClosure());

        for (const auto& field : definition.getFields()) {
            this->m_environment.define(field.getName(), std::make_shared<FalxNull>(FalxNull()));
        }

        for (const auto& [name, method] : definition.getMethods()) {
            const auto boundMethod = UserFunction(method.getParameters(), method.getStatements(), this->m_environment);
            this->m_methods[name] = std::make_shared<UserFunction>(boundMethod);
        }
    }

    std::shared_ptr<FalxValue> get(const std::string &name) const override {
        if (this->m_methods.contains(name)) {
            return this->m_methods.at(name);
        }

        try {
            return this->m_environment.getLocal(name);
        } catch (std::runtime_error& e) {
            throw e.what();
        }
    }

    void set(const std::string &name, const std::shared_ptr<FalxValue> &value) override {
        try {
            this->m_environment.assignLocal(name, value);
        } catch (std::runtime_error& e) {
            throw;
        }
    }

    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue> &other) const override {
        const auto* temp = dynamic_cast<const FalxStruct*>(other.get());

        if (!temp) {
            return false;
        }

        return temp->m_definition == this->m_definition;
    }

    [[nodiscard]] std::string getTypeName() const override {
        return this->m_definition.getName();
    }
};
