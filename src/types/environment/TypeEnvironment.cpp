//
// Created by SharkBlocks00 on 11/6/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "TypeEnvironment.h"

#include <ranges>
#include <utility>

Symbol::Symbol(std::string name, const std::shared_ptr<const FalxType> &type, const bool _mutable, std::string kind)
    : m_name(std::move(name)), m_type(type), m_mutable(_mutable), m_kind(std::move(kind)) {}

const std::string &Symbol::getName() const {
    return m_name;
}

const std::string &Symbol::getKind() const {
    return m_kind;
}

std::shared_ptr<const FalxType> Symbol::getType() const {
    return m_type;
}

bool Symbol::isMutable() const {
    return m_mutable;
}

void Symbol::setType(const std::shared_ptr<const FalxType> &type) {
    m_type = type;
}

// TypeEnvironment
TypeEnvironment::TypeEnvironment(std::shared_ptr<TypeEnvironment> parent, const bool functionBoundary)
    : m_parent(std::move(parent)), m_functionBoundary(functionBoundary) {}


bool TypeEnvironment::getFunctionBoundary() const {
    return m_functionBoundary;
}

const std::map<std::string, std::shared_ptr<const FalxType> > &TypeEnvironment::getNarrowed() const {
    return m_narrowed;
}

std::shared_ptr<TypeEnvironment> TypeEnvironment::getParent() const {
    return m_parent;
}

const std::set<std::string> &TypeEnvironment::getPending() const {
    return m_pending;
}

const std::map<std::string, StructType> &TypeEnvironment::getStructs() const {
    return m_structs;
}

const std::map<std::string, Symbol> &TypeEnvironment::getSymbols() const {
    return m_symbols;
}


std::shared_ptr<TypeEnvironment> TypeEnvironment::child(const bool &functionBoundary) {
    return std::make_shared<TypeEnvironment>(std::shared_ptr<TypeEnvironment>(this), functionBoundary);
}

Symbol TypeEnvironment::define(const std::string &name, std::shared_ptr<const FalxType> &type, const bool &_mutable,
                               const std::string &kind) {
    if (const auto it = m_symbols.find(name); it != m_symbols.end()) {
        if (const Symbol& existing = it->second; kind == "function" && (existing.getKind() == "function" || existing.getKind() == "builtin")) {
            throw std::runtime_error{"Function '" + name + "' already exists!"};
        }
        throw std::runtime_error{"Variable '" + name + "' already exists!"};
    }

    const auto& symbol = Symbol(name, type, _mutable, kind);
    this->m_symbols[name] = symbol;
    this->m_pending.erase(name);
    return symbol;
}

void TypeEnvironment::redefine(const std::string &name, const std::shared_ptr<const FalxType> &type) {
    this->m_symbols[name].setType(type);
}

std::vector<const TypeEnvironment* > TypeEnvironment::chain() const {
    std::vector<const TypeEnvironment*> scopes;
    auto scope = this;

    while (scope) {
        scopes.push_back(scope);
        scope = scope->getParent().get();
    }

    return scopes;
}

std::optional<std::shared_ptr<Symbol> > TypeEnvironment::lookup(const std::string &name) const {
    for (const auto& scope : this->chain()) {
        if (auto it = scope->m_symbols.find(name); it != scope->m_symbols.end()) {
            return std::make_shared<Symbol>(it->second);
        }
    }
    return nullptr;
}

std::optional<std::shared_ptr<TypeEnvironment> > TypeEnvironment::declaringScope(const std::string &name) const {
    for (const auto& scope : this->chain()) {
        if (auto it = scope->m_symbols.find(name); it != scope->m_symbols.end()) {
            return std::make_shared<TypeEnvironment>(*scope);
        }
    }
    return nullptr;
}

bool TypeEnvironment::isPending(const std::string &name) const {
    bool crossedFunction{false};

    for (const auto& scope : this->chain()) {
        if (auto it = scope->m_pending.find(name); it != scope->m_pending.end() && crossedFunction) {
            return true;
        }
        if (scope->getFunctionBoundary()) {
            crossedFunction = true;
        }
    }
    return false;
}

std::optional<std::shared_ptr<StructType> > TypeEnvironment::findStruct(const std::string &name) const {
    for (const auto& scope : this->chain()) {
        if (auto it = scope->m_structs.find(name); it != scope->m_structs.end()) {
            return std::make_shared<StructType>(it->second);
        }
    }
    return nullptr;
}

std::vector<std::string> TypeEnvironment::structNames() const {
    std::vector<std::string> names;

    for (const auto scopes = this->chain(); const auto& scope : scopes) {
        if (!scope) continue;

        for (const auto &name: scope->getStructs() | std::views::keys) {
            names.push_back(name);
        }
    }

    return names;
}

std::optional<std::shared_ptr<const FalxType> > TypeEnvironment::typeOf(const std::string &name) const {
    const auto& symbol = this->lookup(name);
    if (symbol == nullptr) {
        return nullptr;
    }

    bool crossedFunction{false};

    for (const auto& scope : this->chain()) {
        if (auto it = scope->m_narrowed.find(name); it != scope->m_narrowed.end() && !(crossedFunction && (*std::move(symbol))->isMutable())) {
            return scope->m_narrowed.at(name);
        } if (scope->m_symbols.contains(name)) {
            break;
        } if (scope->getFunctionBoundary()) {
            crossedFunction = true;
        }
    }

    return (*std::move(symbol))->getType();
}

void TypeEnvironment::narrow(const std::string &name, const std::shared_ptr<const FalxType> &type) {
    const auto& symbol = this->lookup(name);
    if (!symbol->get()) {
        return;
    }

    if (type == symbol->get()->getType()) {
        this->m_narrowed.erase(name);

        if (this->typeOf(name) != symbol->get()->getType()) {
            this->m_narrowed.emplace(name, symbol->get()->getType());
        }
    } else {
        this->m_narrowed.emplace(name, type);
    }
}

void TypeEnvironment::narrowAll(const std::map<std::string, std::shared_ptr<const FalxType> > &facts) {
    for (const auto& [name, type] : facts) {
        this->narrow(name, type);
    }
}
