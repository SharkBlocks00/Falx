//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <stdexcept>

class BreakException : public std::runtime_error {
public:
    explicit BreakException() : std::runtime_error("'break' called outside of loop.") {}

};
