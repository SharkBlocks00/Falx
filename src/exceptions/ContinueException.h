//
// Created by SharkBlocks00 on 10/24/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <stdexcept>

class ContinueException : public std::runtime_error {
public:
    explicit ContinueException() : std::runtime_error("'continue' called outside of loop.") {}
};
