//
// Created by SharkBlocks00 on 10/22/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../Expression.h"
#include "../../packages/Callable.h"

class FunctionCallExpression : public Expression {
private:
    SourceLocation m_location;
    std::shared_ptr<Expression> m_callee;
    std::vector<std::shared_ptr<Expression>> m_arguments;
public:
    FunctionCallExpression(const SourceLocation& location, const std::shared_ptr<Expression> &callee, const std::vector<std::shared_ptr<Expression>> &arguments)
        : Expression(location), m_location(location), m_callee(callee), m_arguments(arguments) {}

    [[nodiscard]] std::shared_ptr<FalxValue> evaluate(const Interpreter &interpreter, Environment &environment) const override {
        std::shared_ptr<FalxValue> object = this->m_callee->evaluate(interpreter, environment);

        if (typeid(*object) != typeid(Callable)) {
            throw std::runtime_error("Type mismatch in function call");
        }

        const auto& tmp = static_cast<const Callable&>(*object);

        std::vector<std::shared_ptr<FalxValue>> arguments{};

        for (const auto& arg : this->m_arguments) {
            arguments.push_back(arg->evaluate(interpreter, environment));
        }

        if (arguments.size() != tmp.arity() && tmp.isStrict()) {
            throw std::runtime_error("Function call arguments are not valid");
        }

        return std::make_shared<FalxValue>(tmp.call(interpreter, arguments));
    }

};
