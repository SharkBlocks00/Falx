//
// Created by SharkBlocks00 on 10/31/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../NativeFunction.h"
#include "../../runtime/objects/advanced/FalxString.h"

class TypeOfFunction : public NativeFunction {
public:
    [[nodiscard]] bool isStrict() const override { return true; }
    [[nodiscard]] std::size_t arity() const override { return 1; }

    std::shared_ptr<FalxValue> call(const Interpreter &interpreter, std::vector<std::shared_ptr<FalxValue>> &arguments) const override {
        return std::make_shared<FalxString>(arguments[0]->getTypeName());
    }
};
