//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <iostream>

#include "../Expression.h"

class VariableExpression : public Expression {
private:
    std::string m_name;
public:
    explicit VariableExpression(const SourceLocation& location, std::string name)
        : Expression(location), m_name(std::move(name)) {}

    std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        try {
            return environment.get(this->m_name);
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            throw;
        }
    }

    [[nodiscard]] std::string getName() const { return this->m_name; }
};
