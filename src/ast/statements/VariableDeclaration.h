//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <iostream>

#include "../Expression.h"
#include "../Statement.h"
#include "../types/TypeExpression.h"

class VariableDeclaration : public Statement {
private:
    std::string m_name;
    std::shared_ptr<Expression> m_initializer;
    bool m_mutable;
    bool m_exported;
    std::unique_ptr<Environment> m_exportEnvironment{};
    TypeExpressionPtr m_typeAnnotation;
public:
    explicit VariableDeclaration(const SourceLocation &location, std::string name, const std::shared_ptr<Expression> &initializer, const bool _mutable, const bool exported, const TypeExpressionPtr &annotation)
        : Statement(location), m_name(std::move(name)), m_initializer(initializer), m_mutable(_mutable), m_exported(exported), m_typeAnnotation(annotation){}

    void execute(const Interpreter &interpreter, Environment &environment) override {
        try {
            const std::shared_ptr<FalxValue> value = this->m_initializer->evaluate(interpreter, environment);
            environment.define(this->m_name, value, this->m_mutable);
            if (this->m_exportEnvironment) {
                this->m_exportEnvironment->define(this->m_name, value, this->m_mutable);
            }
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        };
    }

    void setExportEnvironment(const Environment &environment) {
        m_exportEnvironment = std::make_unique<Environment>(environment);
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
