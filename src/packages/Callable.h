//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <cstddef>
#include <string>
#include <vector>

#include "../runtime/objects/FalxValue.h"
#include "../runtime/interpreter/Interpreter.h"


class Callable : public FalxValue {
public:
    Callable() = default;
    ~Callable() override = default;

    [[nodiscard]] virtual bool isStrict() const { return false; }
    [[nodiscard]] virtual std::size_t arity() const { return 0; }
    [[nodiscard]] virtual FalxValue call(Interpreter interpreter, std::vector<FalxValue> &arguments) {
        return {};
    }

    static std::string getTypeName() {
        return "callable";
    }
};
