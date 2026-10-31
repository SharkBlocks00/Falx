//
// Created by SharkBlocks00 on 10/31/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <iostream>

#include "../NativeFunction.h"
#include "../../runtime/objects/advanced/FalxString.h"

class RequestFunction : public NativeFunction {
public:

    [[nodiscard]] bool isStrict() const override { return false; }
    [[nodiscard]] std::size_t arity() const override { return 1; }

    std::shared_ptr<FalxValue> call(const Interpreter &interpreter, std::vector<std::shared_ptr<FalxValue>> &arguments) const override {
        std::string in;
        if (arguments.empty()) {
            std::cin >> in;
            return std::make_shared<FalxString>(in);
        }
        std::cout << arguments[0]->asString() << "\n";
        std::cin >> in;
        return std::make_shared<FalxString>(in);
    }
};
