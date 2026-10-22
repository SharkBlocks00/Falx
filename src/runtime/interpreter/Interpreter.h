//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../../ast/Statement.h"
#include "../environment/Environment.h"

class Interpreter {
private:
    Environment m_globals{Environment()};
    Environment m_environment = m_globals;

    void execute(const std::vector<std::shared_ptr<Statement>> &program, Environment& environment) const;
public:
    Interpreter() = default;

    void interpret(const std::vector<std::shared_ptr<Statement>> &program);
};
