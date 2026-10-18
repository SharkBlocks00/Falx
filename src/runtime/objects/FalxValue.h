//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include <map>
#include <string>
#include <variant>
#include <memory>
#include <functional>

class NativeFunction;


class FalxValue {
private:
    std::map<std::string, std::shared_ptr<NativeFunction>> m_methods{};
    std::map<std::string, std::function<FalxValue()>> m_properties{};

public:
    virtual ~FalxValue() = default;
    FalxValue() = default;

    [[nodiscard]] virtual std::shared_ptr<FalxValue> get(const std::string &name) const;
    void set(const std::string &name, const FalxValue& value);
    [[nodiscard]] virtual std::shared_ptr<FalxValue> index(const std::shared_ptr<FalxValue> &index) const;

    virtual void indexAssign(const std::shared_ptr<FalxValue> &key, const std::shared_ptr<FalxValue> &value);

    virtual bool operator==(const FalxValue &other) const;

    [[nodiscard]] virtual bool isHashable() const { return false; }
    [[nodiscard]] virtual std::shared_ptr<FalxValue> add(const std::shared_ptr<FalxValue> &other) const;
    [[nodiscard]] std::shared_ptr<FalxValue> minus(const FalxValue &other);
    [[nodiscard]] FalxValue multiply(const FalxValue &other);
    [[nodiscard]] FalxValue divide(const FalxValue &other);
    [[nodiscard]] FalxValue modulo(const FalxValue &other);
    [[nodiscard]] int compareTo(const FalxValue &other) const;
    [[nodiscard]] virtual bool equalsValue(const std::shared_ptr<FalxValue> &other) const;
    [[nodiscard]] std::string getTypeName() const;
    [[nodiscard]] std::variant<int, double> asNumber() const;
    [[nodiscard]] virtual int asInt() const;
    [[nodiscard]] virtual double asDouble() const;
    [[nodiscard]] virtual std::string asString() const;
    [[nodiscard]] virtual bool asBool() const;
};
