//
// Created by SharkBlocks00 on 10/28/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <string>
#include <vector>

#include "FieldDefinition.h"
#include "UserFunction.h"

class StructDefinition {
private:
    std::string m_name;
    std::vector<FieldDefinition> m_fields;
    std::map<std::string, UserFunction> m_methods;
    Environment m_closure;

public:
    explicit StructDefinition(std::string name, std::vector<FieldDefinition> fields, std::map<std::string, UserFunction> methods, Environment closure)
        : m_name(std::move(name)), m_fields(std::move(fields)), m_methods(std::move(methods)), m_closure(std::move(closure)) {}

    std::vector<FieldDefinition> const& getFields() const { return m_fields; }
    std::map<std::string, UserFunction> const& getMethods() const { return m_methods; }
    std::string const& getName() const { return m_name; }
    Environment getClosure() const { return m_closure; }

    bool operator==(const StructDefinition& other) const {
        return other.m_name == m_name && other.m_fields == m_fields && other.m_methods == m_methods;
    }
};
