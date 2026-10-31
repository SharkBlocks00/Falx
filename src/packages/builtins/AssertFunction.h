//
// Created by SharkBlocks00 on 10/31/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../NativeFunction.h"
#include "../../runtime/objects/FalxNull.h"

class AssertFunction : public NativeFunction {
public:
    [[nodiscard]] bool isStrict() const override { return false; }
    [[nodiscard]] std::size_t arity() const override { return 2; }

    std::shared_ptr<FalxValue> call(const Interpreter &interpreter, std::vector<std::shared_ptr<FalxValue>> &arguments) const override {
        if (arguments.empty()) {
            throw std::invalid_argument("assert() requires at least one argument");
        }

        const std::shared_ptr<FalxValue>& condition = arguments[0];

        if (!condition->asBool()) {
            if (arguments.size() > 1) {
                std::string message = arguments[1]->asString();
                throw std::runtime_error("Assertion failed: " + message);
            }
            throw std::runtime_error("Assertion failed");
        }

        return std::make_shared<FalxNull>();
    }
};
