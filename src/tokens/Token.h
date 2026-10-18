//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//


#pragma once
#include "TokenKind.h"

#include <string>
#include <variant>
#include <type_traits>

#include "../ast/SourceLocation.h"

using Literal = std::variant<
    std::monostate,
    bool,
    double,
    int,
    std::string
>;

[[nodiscard]] inline std::string to_string(const Literal& literal) {
    return std::visit([]<typename T0>(const T0& value) -> std::string {
        using T = std::decay_t<T0>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            return "null";
        } else if constexpr (std::is_same_v<T, bool>) {
            return value ? "true" : "false";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else {
            return std::to_string(value);
        }
    }, literal);
}

class Token {
private:
    TokenKind m_tokenKind;
    std::string m_lexeme;
    Literal m_literal;
    SourceLocation m_location;
public:
    Token(TokenKind tokenKind, std::string lexeme, Literal literal, SourceLocation location);

    [[nodiscard]] std::string asString() const;
    [[nodiscard]] TokenKind getTokenKind() const;
    [[nodiscard]] std::string getLexeme() const;
    [[nodiscard]] Literal getLiteral() const;
    [[nodiscard]] SourceLocation getLocation() const;

};
