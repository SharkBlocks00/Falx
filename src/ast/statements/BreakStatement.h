//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Statement.h"
#include "../../exceptions/BreakException.h"

class BreakStatement : public Statement {
public:
    explicit BreakStatement(const SourceLocation& location)
        : Statement(location) {};

    void execute(const Interpreter &interpreter, Environment &environment) override {
        throw BreakException();
    }

    void print(std::ostream &os) const override {
        os << "BreakStatement";
    }
};
