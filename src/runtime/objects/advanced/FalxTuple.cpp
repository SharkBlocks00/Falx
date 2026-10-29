//
// Created by SharkBlocks00 on 10/22/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxTuple.h"
#include <sstream>

std::vector<std::shared_ptr<FalxValue> > FalxTuple::iterate() const {
    return this->m_values;
}

std::shared_ptr<FalxValue> FalxTuple::index(const std::shared_ptr<FalxValue> &index) const {
    if (index->asInt() > this->m_values.size() || index->asInt() < -this->m_values.size()) {
        throw std::out_of_range("FalxTuple::index");
    }
    return this->m_values[index->asInt()];
}

void FalxTuple::indexAssign(const std::shared_ptr<FalxValue> &key, const std::shared_ptr<FalxValue> &value) {
    throw std::runtime_error("Cannot assign to immutable value tuple");
}

void FalxTuple::set(const std::string &name, const std::shared_ptr<FalxValue> &value) {
    throw std::runtime_error("Cannot assign to immutable value tuple");
}

bool FalxTuple::equalsValue(const std::shared_ptr<FalxValue> &other) const {
    if (typeid(other) != typeid(FalxTuple)) {
        return false;
    }

    const auto& tmp = static_cast<const FalxTuple&>(*other);
    return tmp.m_values == this->m_values;
}

std::string FalxTuple::asString() const {
    if (this->m_values.empty()) {
        return "(,,)";
    }
    if (this->m_values.size() == 1) {
        return "(" + this->m_values.at(0)->asString() + ",)";
    }

    std::ostringstream result;
    result << "(";

    for (std::size_t i = 0; i < this->m_values.size(); ++i) {
        if (i > 0) {
            result << ", ";
        }

        result << this->m_values.at(i)->asString();
    }

    result << ")";
    return result.str();
}
