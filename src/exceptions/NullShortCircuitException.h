//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <stdexcept>

class NullShortCircuitException : public std::runtime_error {
public:
    explicit NullShortCircuitException() : std::runtime_error("'null shortcircuit'") {}
};
