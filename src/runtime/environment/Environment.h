//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <map>
#include <string>
#include <memory>
#include "../../ast/Variable.h"

class Environment {
private:
    std::shared_ptr<Environment> m_parent;
    std::map<std::string, Variable> m_variables{};
    bool m_isStandardLibrary;

public:
    explicit Environment(std::shared_ptr<Environment> parent = nullptr, bool isStandardLibrary = false);

    void define(const std::string& name, const std::shared_ptr<FalxValue>& value, bool _mutable = true);
    void assign(const std::string& name, const std::shared_ptr<FalxValue>& value);
    void checkAssignable(const std::string& name) const;
    void assignLocal(const std::string& name, const std::shared_ptr<FalxValue>& value);


    [[nodiscard]] std::shared_ptr<FalxValue> get(const std::string& name) const;
    [[nodiscard]] std::shared_ptr<FalxValue> getLocal(const std::string& name) const;

};
