//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../FalxValue.h"

using Number = std::variant<int, double>;

class FalxNumber : public FalxValue {
private:
    Number m_value;

    [[nodiscard]] Number _add(const Number& a, const Number& b) const;
    [[nodiscard]] Number _minus(const Number& a, const Number& b) const;
    [[nodiscard]] Number _multiply(const Number& a, const Number& b) const;
    [[nodiscard]] Number _divide(const Number& a, const Number& b) const;
    [[nodiscard]] Number _modulo(const Number& a, const Number& b) const;
public:
    explicit FalxNumber(const int& value) : FalxValue(), m_value(value) {}
    explicit FalxNumber(const double& value) : FalxValue(), m_value(value) {}
    explicit FalxNumber(const Number& value) : FalxValue(),  m_value(value) {}

    [[nodiscard]] int asInt() const override;
    [[nodiscard]] double asDouble() const override;
    [[nodiscard]] Number asNumber() const override;

    [[nodiscard]] std::shared_ptr<FalxValue> add(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] std::shared_ptr<FalxValue> minus(const FalxValue &other) override;
    [[nodiscard]] std::shared_ptr<FalxValue> multiply(const FalxValue &other) override;
    [[nodiscard]] std::shared_ptr<FalxValue> divide(const FalxValue &other) override;
    [[nodiscard]] std::shared_ptr<FalxValue> modulo(const FalxValue &other) override;
    [[nodiscard]] int compareTo(const FalxValue &other) const override;
    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue> &other) const override;

};
