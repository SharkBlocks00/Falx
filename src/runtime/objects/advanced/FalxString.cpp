//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxString.h"

#include "../FalxBoolean.h"

std::string FalxString::asString() const {
    return this->m_value;
}

bool FalxString::asBool() const {
    return this->m_value == "true";
}

int FalxString::asInt() const {
    try {
        return std::stoi(this->m_value);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("FalxString::asInt()-m_value is not a valid integer.");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("FalxString::asInt()-m_value is out of range.");
    }
}

double FalxString::asDouble() const {
    try {
        return std::stod(this->m_value);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("FalxString::asDouble()-m_value is not a valid number.");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("FalxString::asDouble()-m_value is out of range.");
    }
}

bool FalxString::equalsValue(const std::shared_ptr<FalxValue> &other) const {
    if (typeid(*this) != typeid(other)) { return false; }
    const auto temp = static_cast<const FalxString&>(*other);
    return this->m_value == temp.m_value;
}

std::vector<std::shared_ptr<FalxValue>> FalxString::iterate() const {
    std::vector<std::shared_ptr<FalxValue>> result{};

    for (const char& c : this->m_value) {
        result.push_back(std::make_unique<FalxValue>(FalxString(std::to_string(c))));
    }
    return result;
}

std::shared_ptr<FalxValue> FalxString::index(const std::shared_ptr<FalxValue> &index) const {
    const auto i = index->asInt();
    if (i < -this->m_value.size() || i > this->m_value.size()) {
        throw std::out_of_range("FalxString::index()-m_value is out of range.");
    }
    return std::make_unique<FalxValue>(FalxString{std::to_string(this->m_value[i])});
}

std::shared_ptr<FalxValue> FalxString::add(const std::shared_ptr<FalxValue> &other) const {
    if (typeid(*this) == typeid(other)) {
        return std::make_unique<FalxValue>(FalxString(this->m_value + other->asString()));
    } if (typeid(FalxBoolean{true}) == typeid(other)) {
        return std::make_unique<FalxValue>(FalxString(this->m_value + other->asString()));
    }
    return FalxValue::add(other);
}
