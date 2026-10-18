//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../FalxIterable.h"
#include "../FalxValue.h"

class FalxMap : public FalxValue, public FalxIterable {
private:
    std::map<std::shared_ptr<FalxValue>, std::shared_ptr<FalxValue>> m_values{};
public:
    explicit FalxMap(const std::map<std::shared_ptr<FalxValue>, std::shared_ptr<FalxValue>>& values)
        : FalxValue(), FalxIterable(), m_values(values) {}

    [[nodiscard]] std::shared_ptr<FalxValue> get(const std::string& name) const override;
    [[nodiscard]] std::shared_ptr<FalxValue> index(const std::shared_ptr<FalxValue>& index) const override;
    [[nodiscard]] std::vector<std::shared_ptr<FalxValue>> iterate() const override;
    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue>& other) const override;
    [[nodiscard]] std::map<std::shared_ptr<FalxValue>, std::shared_ptr<FalxValue>> getValues() const { return this->m_values; }

    void indexAssign(const std::shared_ptr<FalxValue>& key, const std::shared_ptr<FalxValue>& value) override;
    [[nodiscard]] std::string getTypeName() const override { return "map"; }
    void set(const std::string &name, const std::shared_ptr<FalxValue>& value);

};
