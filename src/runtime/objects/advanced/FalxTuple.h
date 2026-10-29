//
// Created by SharkBlocks00 on 10/22/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include "../FalxIterable.h"
#include "../FalxValue.h"

class FalxTuple : public FalxValue, public FalxIterable {
private:
    std::vector<std::shared_ptr<FalxValue> > m_values;
public:
    explicit FalxTuple(std::vector<std::shared_ptr<FalxValue> > values)
        : FalxValue(), FalxIterable(), m_values(std::move(values)) {}

    [[nodiscard]] std::vector<std::shared_ptr<FalxValue>> iterate() const override;
    [[nodiscard]] std::shared_ptr<FalxValue> index(const std::shared_ptr<FalxValue> &index) const override;

    void indexAssign(const std::shared_ptr<FalxValue> &key, const std::shared_ptr<FalxValue> &value) override;
    void set(const std::string &name, const std::shared_ptr<FalxValue> &value) override;

    [[nodiscard]] bool equalsValue(const std::shared_ptr<FalxValue> &other) const override;

    std::string getTypeName() const override { return "tuple"; }

    std::string asString() const override;

    std::vector<std::shared_ptr<FalxValue>> getValues() const { return m_values; }
};
