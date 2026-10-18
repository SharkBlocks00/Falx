//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "Callable.h"

class NativeFunction : public Callable {
};

inline std::string to_string(const NativeFunction& function) {
    return "<native function>";
}