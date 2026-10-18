//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../FalxIterable.h"
#include "../FalxValue.h"

class FalxString : public FalxValue, public FalxIterable {
private:
    std::string m_value;
public:
    explicit FalxString(std::string& value) : m_value(std::move(value)) {}

    explicit FalxString(const std::string& string) : m_value(string) {}

    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] int asInt() const override;
    [[nodiscard]] double asDouble() const override;
    [[nodiscard]] bool asBool() const override;
    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] std::vector<std::shared_ptr<FalxValue>> iterate() const override;
    [[nodiscard]] std::shared_ptr<FalxValue> index(const std::shared_ptr<FalxValue> &index) const override;
    [[nodiscard]] std::shared_ptr<FalxValue> add(const std::shared_ptr<FalxValue> &other) const override;


    static std::string getTypeName() { return "string"; }
};
