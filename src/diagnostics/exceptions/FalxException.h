//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <memory>
#include <optional>
#include <stdexcept>

#include "../../ast/SourceLocation.h"

class FalxException : public std::exception {
private:
    std::string m_message;
    std::optional<SourceLocation> m_location;
    std::shared_ptr<FalxException> m_cause;
public:
    explicit FalxException(std::string message, std::optional<SourceLocation> location = std::nullopt, std::shared_ptr<FalxException> cause = nullptr)
        : m_message(std::move(message)), m_location(std::move(location)), m_cause(std::move(cause)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return m_message.c_str();
    }

    FalxException& withLocation(const SourceLocation& location) {
        if (!this->m_location.has_value()) {
            this->m_location = location;
        }

        return *this;
    }
};
