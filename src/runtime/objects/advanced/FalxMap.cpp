//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxMap.h"

#include <ranges>

#include "FalxString.h"
#include "../FalxNull.h"

std::shared_ptr<FalxValue> FalxMap::get(const std::string &name) const {
    if (!this->m_values.contains(std::make_unique<FalxValue>(FalxString{name}))) {
        try {
            return FalxValue::get(name);
        } catch (std::runtime_error& e) {
            const auto it = this->m_values.find(std::make_unique<FalxValue>(FalxString{name}));
            if (it != this->m_values.end()) {
                return it->second;
            }
            return std::make_unique<FalxValue>(FalxNull{});
        }
    }

    const auto it = this->m_values.find(std::make_unique<FalxValue>(FalxString{name}));
    if (it != this->m_values.end()) {
        return it->second;
    }
    return std::make_unique<FalxValue>(FalxNull{});
}

std::shared_ptr<FalxValue> FalxMap::index(const std::shared_ptr<FalxValue> &index) const {
    const auto it = this->m_values.find(index);
    if (it != this->m_values.end()) {
        return it->second;
    }
    return std::make_unique<FalxValue>(FalxNull{});
}

void FalxMap::indexAssign(const std::shared_ptr<FalxValue> &key, const std::shared_ptr<FalxValue> &value) {
    if (!key->isHashable()) {
        throw std::runtime_error{"Cannot hash " + key->asString()};
    }
    this->m_values[key] = value;
}

std::vector<std::shared_ptr<FalxValue> > FalxMap::iterate() const {
    std::vector<std::shared_ptr<FalxValue>> result{};

    for (const auto &val: this->m_values | std::views::values) {
        result.push_back(val);
    }
    return result;
}

bool FalxMap::equalsValue(const std::shared_ptr<FalxValue> &other) const {
    const auto* temp = dynamic_cast<const FalxMap*>(other.get());
    if (!temp) {
        return false;
    }
    return this->m_values == temp->m_values;
}

void FalxMap::set(const std::string &name, const std::shared_ptr<FalxValue> &value) {
    this->m_values[std::make_unique<FalxValue>(FalxString{name})] = value;
}
