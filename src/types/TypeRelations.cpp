//
// Created by SharkBlocks00 on 11/6/2026.
// Copyright (c) Sharx All rights reserved.
//

#include <functional>

#include "falxtype/FalxType.h"

bool functionFits(const FunctionType& target, const FunctionType& source);

bool isAssignable(const std::shared_ptr<const FalxType>& target, const std::shared_ptr<const FalxType>& source) {
    if (*target == *source || *target == *ANY || *source == *ANY || *source == *NEVER) {
        return true;
    }

    if (*source == *VOID) {
        return *target == *VOID;
    }

    if (const auto* expr = dynamic_cast<const UnionType*>(source.get())) {
        if (std::all_of(expr->getMembers().cbegin(), expr->getMembers().cend(),
            [target](const auto& member ) { return isAssignable(target, member); }
        )) {
            return true;
        }
        return false;
    }

    if (const auto* expr = dynamic_cast<const UnionType*>(target.get())) {
        if (std::any_of(expr->getMembers().cbegin(), expr->getMembers().cend(),
            [source](const auto& member ) { return isAssignable(member, source); })) {
            return true;
        }
        return false;
    }

    if (*target == *FLOAT_LITERAL && *source == *INTEGER_LITERAL) {
        return true;
    }

    if (const auto* a = dynamic_cast<const ArrayType*>(target.get());
        const auto* b = dynamic_cast<const ArrayType*>(source.get())) {
        return isAssignable(a->getElementType(), b->getElementType());
    }

    if (const auto* a = dynamic_cast<const MapType*>(target.get());
        const auto* b = dynamic_cast<const MapType*>(source.get())) {
            return isAssignable(a->getKeyType(), b->getKeyType())
            && isAssignable(a->getValueType(), b->getValueType());
    }

    if (const auto* a = dynamic_cast<const TupleType*>(target.get());
        const auto* b = dynamic_cast<const TupleType*>(source.get())) {
        if (a->getElementTypes().size() != b->getElementTypes().size()) {
            return false;
        }

        for (std::size_t i = 0; i < a->getElementTypes().size(); ++i) {
            if (!isAssignable(a->getElementTypes()[i], b->getElementTypes()[i])) {
                return false;
            }
        }
        return true;
    }

    if (const auto* a = dynamic_cast<const FunctionType*>(target.get());
        const auto* b = dynamic_cast<const FunctionType*>(source.get())) {
        return functionFits(*a, *b);
    }

    if (const auto* a = dynamic_cast<const StructType*>(target.get());
        const auto* b = dynamic_cast<const StructType*>(source.get())) {
        return a->getInfo() == b->getInfo();
    }

    return false;
}

bool functionFits(const FunctionType& target, const FunctionType& source) {
    const std::size_t given = target.getParameterTypes().size();

    if (given > source.getMaxArguments() || given < source.getMinArguments()) {
        return false;
    }

    for (std::size_t i = 0; i < given; ++i) {
        if (!isAssignable(source.getParameterTypes()[i], target.getParameterTypes()[i])) {
            return false;
        }
    }

    return *target.getReturnType() == *VOID || isAssignable(target.getReturnType(), source.getReturnType());
}

bool acceptsNull(const std::shared_ptr<const FalxType>& type) {
    return isAssignable(type, NULL_LITERAL);
}