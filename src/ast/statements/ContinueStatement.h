//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Statement.h"
#include "../../exceptions/ContinueException.h"

class ContinueStatement : public Statement {
public:
    explicit ContinueStatement(const SourceLocation& location)
        : Statement(location) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        throw ContinueException();
    }

    void print(std::ostream &os) const override {
        os << "ContinueStatement.h";
    }
};
