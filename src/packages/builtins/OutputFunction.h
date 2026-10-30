//
// Created by SharkBlocks00 on 10/30/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <iostream>

#include "../NativeFunction.h"
#include "../../runtime/objects/FalxNull.h"

class OutputFunction : public NativeFunction {
public:
    bool isStrict() const override { return false; }
    bool isHashable() const override { return true; }
    std::size_t arity() const override { return 1; }

    std::shared_ptr<FalxValue> call(const Interpreter &interpreter, std::vector<std::shared_ptr<FalxValue>> &arguments) const override {
        if (arguments.empty()) {
            std::cout << "\n";
            return std::make_shared<FalxNull>();
        }

        std::cout << arguments.at(0)->asString() << "\n";
        return std::make_shared<FalxNull>();
    }
};
