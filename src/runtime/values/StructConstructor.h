//
// Created by SharkBlocks00 on 10/28/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "StructDefinition.h"
#include "../../packages/Callable.h"
#include "../objects/advanced/FalxStruct.h"

class StructConstructor : public Callable {
private:
    StructDefinition m_definition;
public:
    [[nodiscard]] bool isStrict() const override { return false; }
    [[nodiscard]] std::size_t arity() const override { return this->m_definition.getFields().size(); }

    explicit StructConstructor(StructDefinition  definition) : m_definition(std::move(definition)) {}

    std::shared_ptr<FalxValue> call(const Interpreter &interpreter, std::vector<std::shared_ptr<FalxValue>> &arguments) const override {
        std::size_t requiredFields{0};
        for (const auto& field : this->m_definition.getFields()) {
            if (!field.getDefaultValue()) {
                requiredFields++;
            }
        }

        if (arguments.size() != requiredFields && arguments.size() > this->m_definition.getFields().size()) {
            throw std::invalid_argument("Invalid number of arguments when constructing a struct");
        }

        auto _struct = FalxStruct(this->m_definition);
        std::vector<FieldDefinition> fields = this->m_definition.getFields();

        for (size_t i = 0; i < this->m_definition.getFields().size(); i++) {
            const FieldDefinition& field = this->m_definition.getFields()[i];

            std::shared_ptr<FalxValue> value;

            if (i < arguments.size()) {
                value = arguments[i];
            } else if (field.getDefaultValue()) {
                auto tmp = Environment(this->m_definition.getClosure());
                value = field.getDefaultValue()->evaluate(interpreter, tmp);
            } else {
                value = std::make_shared<FalxNull>();
            }

            _struct.set(field.getName(), value);
        }

        return std::make_shared<FalxStruct>(_struct);
    }
};
