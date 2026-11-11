//
// Created by SharkBlocks00 on 11/7/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include "falxtype/FalxType.h"
#include "../ast/expressions/BinaryExpression.h"


using Facts = std::map<std::string, std::shared_ptr<const FalxType>>;
using TypeLookup = std::function<std::optional<std::shared_ptr<const FalxType>>(const std::string&)>;

std::pair<Facts, Facts> conditionFacts(const std::shared_ptr<Expression>& condition, const TypeLookup& lookup);
std::pair<Facts, Facts> _nullComparison(const BinaryExpression* condition, const TypeLookup& lookup);
Facts _both(const Facts& first, const Facts& second);
Facts _either(const Facts& first, const Facts& second);