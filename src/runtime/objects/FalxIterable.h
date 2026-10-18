//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <memory>
#include <vector>

#include "FalxValue.h"

class FalxIterable {
public:
    virtual ~FalxIterable() = default;

    [[nodiscard]] virtual std::vector<std::shared_ptr<FalxValue>> iterate() const = 0;
};
