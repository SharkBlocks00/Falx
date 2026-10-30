//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxArray.h"

FalxArray::FalxArray(const std::vector<std::shared_ptr<FalxValue> > &values)
    : FalxValue(), FalxIterable(), m_values(values) {}

std::vector<std::shared_ptr<FalxValue>> FalxArray::iterate() const {
    return this->m_values;
}

std::shared_ptr<FalxValue> FalxArray::index(const std::shared_ptr<FalxValue> &index) {
    const int i = index->asInt();
    if (i >= this->m_values.size() || i < -this->m_values.size()) {
        throw std::out_of_range("FalxArray::index");
    }
    if (i == 0 && this->m_values.empty()) { return this->m_values.at(0); }

    return this->m_values.at(i);
}

void FalxArray::indexAssign(const std::shared_ptr<FalxValue> &index, const std::shared_ptr<FalxValue>& value) {
    const int i = index->asInt();
    if (i >= this->m_values.size() || i < -this->m_values.size()) {
        throw std::out_of_range("FalxArray::indexAssign");
    }
    this->m_values.at(i) = value;
}

bool FalxArray::equalsValue(const std::shared_ptr<FalxValue> &other) const {
    const auto* temp = dynamic_cast<const FalxArray*>(other.get());
    if (!temp) { return false; }
    return temp->iterate() == this->m_values;
}

