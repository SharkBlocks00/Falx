//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../FalxValue.h"

using Number = std::variant<int, double>;

inline int to_int(const Number& number) {
    return std::visit([](const auto& value) -> int {
        return static_cast<int>(value);
    }, number);
}

inline std::string to_string(const Number& number) {
    if (std::holds_alternative<int>(number)) { return std::to_string(std::get<int>(number)); }
    return std::to_string(std::get<double>(number));
}

inline double to_double(const Number& number) {
    return std::visit([](const auto& value) -> double {
        return static_cast<double>(value);
    }, number);
}

inline std::string repeat(const std::string& str, const int count) {
    std::string result;
    for (int i = 0; i < count; ++i) {
        result += str;
    }
    return result;
}

class FalxNumber : public FalxValue {
private:
    Number m_value;

    [[nodiscard]] static Number _add(const Number& a, const Number& b);
    [[nodiscard]] static Number _minus(const Number& a, const Number& b);
    [[nodiscard]] static Number _multiply(const Number& a, const Number& b);
    [[nodiscard]] static Number _divide(const Number& a, const Number& b);
    [[nodiscard]] static Number _modulo(const Number& a, const Number& b);
public:
    explicit FalxNumber(const int& value) : FalxValue(), m_value(value) {}
    explicit FalxNumber(const double& value) : FalxValue(), m_value(value) {}
    explicit FalxNumber(const Number& value) : FalxValue(),  m_value(value) {}

    std::string asString() const override;
    [[nodiscard]] int asInt() const override;
    [[nodiscard]] double asDouble() const override;
    [[nodiscard]] Number asNumber() const override;

    [[nodiscard]] std::shared_ptr<FalxValue> add(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] std::shared_ptr<FalxValue> minus(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] std::shared_ptr<FalxValue> multiply(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] std::shared_ptr<FalxValue> divide(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] std::shared_ptr<FalxValue> modulo(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] int compareTo(const std::shared_ptr<FalxValue> &other) const override;
    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue> &other) const override;

    [[nodiscard]] std::string getTypeName() const override { return "number"; }
};
