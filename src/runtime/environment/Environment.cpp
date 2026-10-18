//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "Environment.h"

#include <utility>

Environment::Environment(std::shared_ptr<Environment> parent, const bool isStandardLibrary)
    : m_parent(std::move(parent))
{
    this->m_isStandardLibrary = isStandardLibrary || (parent != nullptr && parent);
}

void Environment::define(const std::string& name, const std::shared_ptr<FalxValue>& value, const bool _mutable) {
    if (this->m_variables.contains(name)) {
        throw std::runtime_error{name + " is already defined."};
    }

    this->m_variables.insert({name, Variable(value, _mutable)});
}

void Environment::assign(const std::string& name, const std::shared_ptr<FalxValue>& value) {
    if (const auto it = this->m_variables.find(name); it != this->m_variables.end()) {
        if (!it->second.isMutable()) {
            throw std::runtime_error{name + " is immutable."};
        }
        it->second.setValue(value);
        return;
    }

    if (this->m_parent != nullptr) {
        this->m_parent->assign(name, value);
        return;
    }

    throw std::runtime_error{name + " is not defined."};
}

void Environment::checkAssignable(const std::string& name) {
    if (const auto it{this->m_variables.find(name)}; it != this->m_variables.end()) {
        if (!it->second.isMutable()) {
            throw std::runtime_error{name + " is immutable."};
        }
        return;
    }

    if (this->m_parent != nullptr) {
        this->m_parent->checkAssignable(name);
        return;
    }

    throw std::runtime_error{name + " is not defined."};
}

std::shared_ptr<FalxValue> Environment::get(const std::string& name) {
    if (const auto it{this->m_variables.find(name)}; it != this->m_variables.end()) {
        return it->second.getValue();
    }

    if (this->m_parent != nullptr) {
        return this->m_parent->get(name);
    }

    throw std::runtime_error{name + " is not defined."};
}

std::shared_ptr<FalxValue> Environment::getLocal(const std::string &name) {
    if (const auto it{this->m_variables.find(name)}; it != this->m_variables.end()) {
        return it->second.getValue();
    }

    throw std::runtime_error{name + " is not defined."};
}

void Environment::assignLocal(const std::string& name, const std::shared_ptr<FalxValue>& value) {
    if (const auto it{this->m_variables.find(name)}; it != this->m_variables.end()) {
        if (!it->second.isMutable()) {
            throw std::runtime_error{name + " is immutable."};
        }
        it->second.setValue(value);
        return;
    }

    throw std::runtime_error{name + " is not defined."};
}
