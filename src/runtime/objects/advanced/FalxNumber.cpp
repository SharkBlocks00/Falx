//
// Created by SharkBlocks00 on 10/18/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxNumber.h"

#include "FalxString.h"

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
    if (typeid(other) == typeid(FalxString)) {
        return std::make_unique<FalxValue>(FalxString(this->asString() + other->asString()));
    }
    if (typeid(other) == typeid(FalxNumber)) {
        return std::make_unique<FalxValue>(FalxNumber(_add(this->m_value, other->asNumber())));
    }

    return FalxValue::add(other);
}

std::shared_ptr<FalxValue> FalxNumber::minus(const std::shared_ptr<FalxValue> &other) const {
    if (typeid(other) == typeid(FalxNumber)) {
        return std::make_unique<FalxValue>(FalxNumber(_minus(this->m_value, other->asNumber())));
    }

    return FalxValue::minus(other);
}

std::shared_ptr<FalxValue> FalxNumber::multiply(const std::shared_ptr<FalxValue> &other) const {
    if (typeid(other) == typeid(FalxNumber)) {
        return std::make_unique<FalxValue>(FalxNumber(_multiply(this->m_value, other->asNumber())));
    }

    if (typeid(other) == typeid(FalxString)) {
        return std::make_unique<FalxValue>(FalxString(repeat(other->asString(), this->asInt())));
    }

    return FalxValue::multiply(other);
}

std::shared_ptr<FalxValue> FalxNumber::divide(const std::shared_ptr<FalxValue> &other) const {
    if (typeid(other) == typeid(FalxNumber)) {
        return std::make_unique<FalxValue>(FalxNumber(_divide(this->m_value, other->asNumber())));
    }

    return FalxValue::divide(other);
}

std::shared_ptr<FalxValue> FalxNumber::modulo(const std::shared_ptr<FalxValue> &other) const {
    if (typeid(other) == typeid(FalxNumber)) {
        return std::make_unique<FalxValue>(FalxNumber(_modulo(this->m_value, other->asNumber())));
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
    if (typeid(other) != typeid(FalxNumber)) {
        return false;
    }
    return this->asDouble() == other->asDouble();
}

Number FalxNumber::_add(const Number &a, const Number &b) {
    if (typeid(a) == typeid(int) && typeid(b) == typeid(int)) {
        return to_int(a) + to_int(b);
    }
    return to_double(a) + to_double(b);
}

Number FalxNumber::_minus(const Number &a, const Number &b) {
    if (typeid(a) == typeid(int) && typeid(b) == typeid(int)) {
        return to_int(a) - to_int(b);
    }
    return to_double(a) - to_double(b);
}

Number FalxNumber::_multiply(const Number &a, const Number &b) {
    if (typeid(a) == typeid(int) && typeid(b) == typeid(int)) {
        return to_int(a) * to_int(b);
    }
    return to_double(a) * to_double(b);
}

Number FalxNumber::_divide(const Number &a, const Number &b) {
    if (typeid(a) == typeid(int) && typeid(b) == typeid(int)) {
        return to_int(a) / to_int(b);
    }
    return to_double(a) / to_double(b);
}

Number FalxNumber::_modulo(const Number &a, const Number &b) {
    if (typeid(a) == typeid(int) && typeid(b) == typeid(int)) {
        return to_int(a) % to_int(b);
    }
    return to_double(a) / to_double(b);
}


