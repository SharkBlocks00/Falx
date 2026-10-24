//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxValue.h"
#include "../../packages/Callable.h"
#include "../../packages/NativeFunction.h"

std::shared_ptr<FalxValue> FalxValue::get(const std::string &name) const {
    if (const auto it{this->m_properties.find(name)}; it != this->m_properties.end()) {
        return std::make_unique<FalxValue>(it->second());
    }

    if (const auto it{this->m_methods.find(name)}; it != this->m_methods.end()) {
        return std::make_unique<NativeFunction>(static_cast<NativeFunction>(*it->second));
    }

    throw std::runtime_error{this->getTypeName() + " has no property '" + name + "'"};

}

void FalxValue::set(const std::string &name, const std::shared_ptr<FalxValue> &value) {
    throw std::runtime_error{this->getTypeName() + " has no settable property '" + name + "'"};
}

std::shared_ptr<FalxValue> FalxValue::index(const std::shared_ptr<FalxValue> &index) const {
    throw std::runtime_error{this->getTypeName() + " is not indexable."};
}

void FalxValue::indexAssign(const std::shared_ptr<FalxValue>& key, const std::shared_ptr<FalxValue>& value) {
    throw std::runtime_error{this->getTypeName() + " is not indexable."};
}

bool FalxValue::operator==(const FalxValue& other) const {
    if (this->getTypeName() != other.getTypeName()) {
        return false;
    }
    return true;
}

std::string FalxValue::getTypeName() const {
    return "object";
}

std::shared_ptr<FalxValue> FalxValue::add(const std::shared_ptr<FalxValue>& other) const {
    throw std::runtime_error{""};
}

std::shared_ptr<FalxValue> FalxValue::minus(const std::shared_ptr<FalxValue>& other) const {
    throw std::runtime_error{""};
}

std::shared_ptr<FalxValue> FalxValue::multiply(const std::shared_ptr<FalxValue>& other) const {
    throw std::runtime_error{""};
}

std::shared_ptr<FalxValue> FalxValue::divide(const std::shared_ptr<FalxValue>& other) const {
    throw std::runtime_error{""};
}

std::shared_ptr<FalxValue> FalxValue::modulo(const std::shared_ptr<FalxValue>& other) const {
    throw std::runtime_error{""};
}

int FalxValue::compareTo(const std::shared_ptr<FalxValue>& other) const {
    throw std::runtime_error{""};
}

bool FalxValue::equalsValue(const std::shared_ptr<FalxValue>& other) const {
    return this == &*other;
}

std::variant<int, double> FalxValue::asNumber() const {
    throw std::runtime_error{""};
}

int FalxValue::asInt() const {
    throw std::runtime_error{""};
}

bool FalxValue::asBool() const {
    throw std::runtime_error{""};
}

double FalxValue::asDouble() const {
    throw std::runtime_error{""};
}

std::string FalxValue::asString() const {
    return this->getTypeName();
}

