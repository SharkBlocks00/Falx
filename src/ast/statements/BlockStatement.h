//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <utility>

#include "../Statement.h"

class BlockStatement : public Statement {
private:
    std::vector<std::shared_ptr<Statement>> m_statements;
public:
    explicit BlockStatement(const SourceLocation& location, std::vector<std::shared_ptr<Statement>> statements)
    : Statement(location), m_statements(std::move(statements)) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        auto block = Environment(environment);
        for (const auto &statement : m_statements) {
            statement->execute(interpreter, block);
        }
    }

    void print(std::ostream &os) const override {
        os << "BlockStatement";
    }
};
