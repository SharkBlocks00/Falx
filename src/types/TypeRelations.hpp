//
// Created by SharkBlocks00 on 11/7/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include "falxtype/FalxType.h"

bool isAssignable(const std::shared_ptr<const FalxType>& target, const std::shared_ptr<const FalxType>& source);
bool functionFits(const FunctionType& target, const FunctionType& source);
bool acceptsNull(const std::shared_ptr<const FalxType>& type);