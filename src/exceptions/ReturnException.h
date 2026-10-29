//
// Created by SharkBlocks00 on 10/26/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <stdexcept>
#include <utility>

#include "../runtime/objects/FalxValue.h"

class ReturnException : public std::runtime_error {
private:
    std::shared_ptr<FalxValue> m_value;
public:
    explicit ReturnException(std::shared_ptr<FalxValue> value) : std::runtime_error("Return"), m_value(std::move(value)) {
    }

    [[nodiscard]] std::shared_ptr<FalxValue> getValue() { return m_value; }
};
