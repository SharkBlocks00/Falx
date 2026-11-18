//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxRuntimeException.h"

class IndexInvalidException : public FalxRuntimeException {
public:
    explicit IndexInvalidException(const std::variant<std::string, std::int64_t, int, std::size_t>& index, const std::size_t size, std::optional<std::string> type = std::nullopt)
        : FalxRuntimeException(
            "Index '" + std::visit([]<typename T>(const T& value) -> std::string {
                if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
                    return value;
                } else {
                    return std::to_string(value);
                }
            }, index) + "' is invalid for " + type.value_or("structure") + " with size '" + std::to_string(size) + "'") {}

    template<typename Exception>
    static void check(std::int64_t index, const std::size_t size) {
        if (size == 0) {
            throw Exception{index, size};
        }

        const auto signedSize = static_cast<std::int64_t>(size);

        if (index < -signedSize || index >= signedSize) {
            throw Exception{index, size};
        }
    }

    static std::size_t normalize(std::int64_t index, const std::size_t size) {
        check<IndexInvalidException>(index, size);

        if (index < 0) {
            index += static_cast<std::int64_t>(size);
        }

        return static_cast<std::size_t>(index);
    }
};


class ArrayIndexInvalidException : public IndexInvalidException {
public:
    explicit ArrayIndexInvalidException(const std::variant<std::string, std::int64_t, int, std::size_t>& index, const std::size_t& size)
        : IndexInvalidException(index, size, "array") {}

    static void check(const std::int64_t index, const std::size_t size) {
        IndexInvalidException::check<ArrayIndexInvalidException>(index, size);
    }
};

class MapIndexInvalidException : public IndexInvalidException {
public:
    explicit MapIndexInvalidException(const std::variant<std::string, std::int64_t, int, std::size_t>& index, const std::size_t& size)
        : IndexInvalidException(index, size, "map") {}

    static void check(const std::int64_t index, const std::size_t size) {
        IndexInvalidException::check<MapIndexInvalidException>(index, size);
    }
};

class StringIndexInvalidException : public IndexInvalidException {
public:
    explicit StringIndexInvalidException(const std::variant<std::string, std::int64_t, int, std::size_t>& index, const std::size_t size)
        : IndexInvalidException(index, size, "string") {}

    static void check(const std::int64_t index, const std::size_t size) {
        IndexInvalidException::check<StringIndexInvalidException>(index, size);
    }
};


class CannotHashObjectException : public FalxRuntimeException {
public:
    explicit CannotHashObjectException(const std::string& object, const std::string& type, const std::optional<SourceLocation>& location = std::nullopt)
        : FalxRuntimeException("Cannot hash " + type + " with value of '" + object + "'", location) {}
};

class ObjectIsNotIndexableException : public FalxRuntimeException {
public:
    explicit ObjectIsNotIndexableException(const std::string& object)
        : FalxRuntimeException("'" + object + "' is not indexable") {}
};