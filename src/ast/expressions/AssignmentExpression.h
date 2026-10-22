//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <iostream>

#include "../Expression.h"

class AssignmentExpression : public Expression {
private:
    std::string m_name;
    std::shared_ptr<Expression> m_value;
public:
    AssignmentExpression(const SourceLocation& location, std::string name, std::shared_ptr<Expression>& value)
        : Expression(location), m_name(std::move(name)), m_value(std::move(value)) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        const std::shared_ptr<FalxValue> newValue = this->m_value->evaluate(interpreter, environment);
        try {
            environment.assign(this->m_name, newValue);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        return newValue;
    }

    void print(std::ostream &os) const override {
        os << m_name;

        if (m_value) {
            os << " = ";
            m_value->print(os);
        }
        os << ';';
    }
};
