//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxValue.h"

class FalxBoolean : public FalxValue {
private:
    bool m_value;
public:
    explicit FalxBoolean(const bool value) : m_value(value) {}

    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue>& other) const {
        if (typeid(other) != typeid(*this)) { return false; }
        const auto temp = static_cast<const FalxBoolean&>(*other);

        return this->m_value == temp.getValue();
    }

    [[nodiscard]] bool getValue() const { return this->m_value; }
    [[nodiscard]] std::string asString() const override { return this->m_value ? "true" : "false"; }
    [[nodiscard]] int asInt() const override { return std::stoi(this->asString()); }
    [[nodiscard]] double asDouble() const override { return std::stod(this->asString()); }
    [[nodiscard]] std::string getTypeName() const override { return "boolean"; }
};
