//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../runtime/objects/FalxValue.h"

class Variable : public FalxValue {
private:
    std::shared_ptr<FalxValue> m_value;
    bool m_mutable{true};
public:
    explicit Variable(std::shared_ptr<FalxValue> value, const bool _mutable = true)
        : m_value(std::move(value)), m_mutable(_mutable) {}

    bool operator==(const FalxValue &other) const override {
        if (typeid(*this) != typeid(other)) return false;

        const auto &otherVar = static_cast<const Variable&>(other);

        return *(this->m_value) == *(otherVar.m_value) && this->m_mutable == otherVar.m_mutable;
    }

    [[nodiscard]] std::shared_ptr<FalxValue> getValue() const {
        return m_value;
    }

    void setValue(const std::shared_ptr<FalxValue>& value) {
        m_value = value;
    }

    [[nodiscard]] bool isMutable() const {
        return m_mutable;
    }

    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue> &other) const override {
        if (const auto* o = dynamic_cast<const Variable *>(other.get())) {
            return o->getValue() == getValue() && o->isMutable() == isMutable();
        }
        return false;
    }
};


inline std::string to_string(const Variable &var) {
    return var.asString();
}