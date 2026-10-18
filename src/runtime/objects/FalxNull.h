//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxValue.h"

class FalxNull : public FalxValue {
public:
    FalxNull() : FalxValue() {};

    [[nodiscard]] std::shared_ptr<FalxValue> INSTANCE() { return std::make_shared<FalxNull>(*this); }
    [[nodiscard]] std::string asString() const override { return "null"; }
    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue> &other) const override {
        return typeid(*this) == typeid(other);
    }

    static std::string getTypeName() { return "null"; }

};
