//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxNumber.h"

#include "FalxString.h"

std::string FalxNumber::asString() const {
    return to_string(this->m_value);
}

int FalxNumber::asInt() const {
    return to_int(this->m_value);
}

double FalxNumber::asDouble() const {
    return to_double(this->m_value);
}

Number FalxNumber::asNumber() const {
    return this->m_value;
}

std::shared_ptr<FalxValue> FalxNumber::add(const std::shared_ptr<FalxValue> &other) const {
    if (const auto* expr = dynamic_cast<const FalxString*>(other.get())) {
        return std::make_shared<FalxString>(this->asString() + expr->asString());
    }
    if (const auto* expr = dynamic_cast<const FalxNumber*>(other.get())) {
        return std::make_shared<FalxNumber>(_add(this->m_value, expr->asNumber()));
    }

    return FalxValue::add(other);
}

std::shared_ptr<FalxValue> FalxNumber::minus(const std::shared_ptr<FalxValue> &other) const {
    if (const auto* expr = dynamic_cast<const FalxNumber*>(other.get())) {
        return std::make_shared<FalxNumber>(_minus(this->m_value, expr->asNumber()));
    }

    return FalxValue::minus(other);
}

std::shared_ptr<FalxValue> FalxNumber::multiply(const std::shared_ptr<FalxValue> &other) const {
    if (const auto* expr = dynamic_cast<const FalxNumber*>(other.get())) {
        return std::make_shared<FalxNumber>(_multiply(this->m_value, expr->asNumber()));
    }

    if (const auto* expr = dynamic_cast<const FalxString*>(other.get())) {
        return std::make_shared<FalxString>(repeat(expr->asString(), this->asInt()));
    }

    return FalxValue::multiply(other);
}

std::shared_ptr<FalxValue> FalxNumber::divide(const std::shared_ptr<FalxValue> &other) const {
    if (const auto* expr = dynamic_cast<const FalxNumber*>(other.get())) {
        return std::make_shared<FalxNumber>(_divide(this->m_value, expr->asNumber()));
    }

    return FalxValue::divide(other);
}

std::shared_ptr<FalxValue> FalxNumber::modulo(const std::shared_ptr<FalxValue> &other) const {
    if (const auto* expr = dynamic_cast<const FalxNumber*>(other.get())) {
        return std::make_shared<FalxNumber>(_modulo(this->m_value, expr->asNumber()));
    }

    return FalxValue::modulo(other);
}

int FalxNumber::compareTo(const std::shared_ptr<FalxValue> &other) const {
    const double a = this->asDouble();
    const double b = other->asDouble();

    if (a < b) { return -1; }
    if (b < a) { return 1; }

    return 0;
}

bool FalxNumber::equalsValue(const std::shared_ptr<FalxValue> &other) const {
    if (!dynamic_cast<const FalxNumber*>(other.get())) {
        return false;
    }
    return this->asDouble() == other->asDouble();
}

Number FalxNumber::_add(const Number &a, const Number &b) {
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        return to_int(a) + to_int(b);
    }
    return to_double(a) + to_double(b);
}

Number FalxNumber::_minus(const Number &a, const Number &b) {
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        return to_int(a) - to_int(b);
    }
    return to_double(a) - to_double(b);
}

Number FalxNumber::_multiply(const Number &a, const Number &b) {
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        return to_int(a) * to_int(b);
    }
    return to_double(a) * to_double(b);
}

Number FalxNumber::_divide(const Number &a, const Number &b) {
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        return to_int(a) / to_int(b);
    }
    return to_double(a) / to_double(b);
}

Number FalxNumber::_modulo(const Number &a, const Number &b) {
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        return to_int(a) % to_int(b);
    }
    return to_double(a) / to_double(b);
}


