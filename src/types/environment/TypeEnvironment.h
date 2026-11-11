//
// Created by SharkBlocks00 on 11/6/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <memory>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "../falxtype/FalxType.h"

class Symbol {
private:
    std::string m_name;
    std::shared_ptr<const FalxType> m_type;
    bool m_mutable{false};
    std::string m_kind;
public:
    explicit Symbol() = default;
    explicit Symbol(std::string name, const std::shared_ptr<const FalxType>& type, bool _mutable, std::string kind);

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] std::shared_ptr<const FalxType> getType() const;
    [[nodiscard]] bool isMutable() const;
    [[nodiscard]] const std::string& getKind() const;

    void setType(const std::shared_ptr<const FalxType>& type);
};

class TypeEnvironment {
private:
    std::shared_ptr<TypeEnvironment> m_parent;
    bool m_functionBoundary;
    std::map<std::string, Symbol> m_symbols{};
    std::map<std::string, StructType> m_structs{};
    std::map<std::string, std::shared_ptr<const FalxType>> m_narrowed{};
    std::set<std::string> m_pending{};
public:
    TypeEnvironment() = default;
    explicit TypeEnvironment(std::shared_ptr<TypeEnvironment> parent, bool functionBoundary);

    [[nodiscard]] const std::set<std::string>& getPending() const;
    [[nodiscard]] bool getFunctionBoundary() const;
    [[nodiscard]] const std::map<std::string, Symbol>& getSymbols() const;
    [[nodiscard]] const std::map<std::string, StructType>& getStructs() const;
    [[nodiscard]] const std::map<std::string, std::shared_ptr<const FalxType>>& getNarrowed() const;
    [[nodiscard]] std::shared_ptr<TypeEnvironment> getParent() const;

    [[nodiscard]] std::shared_ptr<TypeEnvironment> child(const bool& functionBoundary);
    [[nodiscard]] Symbol define(const std::string &name, const std::shared_ptr<const FalxType> &type, const bool &_mutable,
                                const std::string &kind = "variable");

    [[nodiscard]] std::vector<const TypeEnvironment*> chain() const;
    [[nodiscard]] std::optional<std::shared_ptr<Symbol>> lookup(const std::string& name) const;
    [[nodiscard]] std::optional<std::shared_ptr<TypeEnvironment>> declaringScope(const std::string& name) const;
    [[nodiscard]] bool isPending(const std::string& name) const;

    [[nodiscard]] std::optional<std::shared_ptr<StructType>> findStruct(const std::string& name) const;
    [[nodiscard]] std::vector<std::string> structNames() const;

    [[nodiscard]] std::optional<std::shared_ptr<const FalxType>> typeOf(const std::string& name) const;

    void narrow(const std::string& name, const std::shared_ptr<const FalxType>& type);
    void narrowAll(const std::map<std::string, std::shared_ptr<const FalxType>>& facts);
    void redefine(const std::string& name, const std::shared_ptr<const FalxType>& type);

};
