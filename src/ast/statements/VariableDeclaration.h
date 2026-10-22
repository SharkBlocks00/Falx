//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <iostream>

#include "../Expression.h"
#include "../Statement.h"

class VariableDeclaration : public Statement {
private:
    std::string m_name;
    std::shared_ptr<Expression> m_initializer;
    bool m_mutable;
    bool m_exported;
public:
    explicit VariableDeclaration(const SourceLocation &location, std::string name, const std::shared_ptr<Expression> &initializer, const bool _mutable, const bool exported)
        : Statement(location), m_name(std::move(name)), m_initializer(std::move(initializer)), m_mutable(_mutable), m_exported(exported) {}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        try {
            const std::shared_ptr<FalxValue> value = this->m_initializer->evaluate(interpreter, environment);
            environment.define(this->m_name, value, this->m_mutable);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        };
    }

    void print(std::ostream &os) const override {
        if (m_exported) {
            os << "export ";
        }

        os << (m_mutable ? "let " : "const ");
        os << m_name;

        if (m_initializer) {
            os << " = ";
            m_initializer->print(os);
        }

        os <<';';
    }
};
