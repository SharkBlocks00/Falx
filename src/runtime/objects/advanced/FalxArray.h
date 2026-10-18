//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "../FalxIterable.h"
#include "../FalxValue.h"

class FalxArray : public FalxValue, public FalxIterable {
private:
    std::vector<std::shared_ptr<FalxValue>> m_values;
public:
    explicit FalxArray(const std::vector<std::shared_ptr<FalxValue>>& values);

    [[nodiscard]] std::vector<std::shared_ptr<FalxValue>> iterate() const override;
    [[nodiscard]] std::shared_ptr<FalxValue> index(const std::shared_ptr<FalxValue>& index);

    void indexAssign(const std::shared_ptr<FalxValue>& index, const std::shared_ptr<FalxValue>& value);
    bool equalsValue(const std::shared_ptr<FalxValue>& other);
    static std::string getTypeName() { return "array"; }
};
