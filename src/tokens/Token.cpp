//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "Token.h"

#include <utility>

Token::Token(const TokenKind tokenKind, std::string lexeme, Literal literal, SourceLocation location)
    : m_tokenKind(tokenKind), m_lexeme(std::move(lexeme)), m_literal(std::move(literal)), m_location(std::move(location)) {}

[[nodiscard]] std::string Token::asString() const {
    return std::string{to_string(m_tokenKind)} + " " + m_lexeme + " " + to_string(m_literal) + " " + m_location.asString();
}

[[nodiscard]] TokenKind Token::getTokenKind() const {
    return m_tokenKind;
}

[[nodiscard]] Literal Token::getLiteral() const {
    return m_literal;
}

[[nodiscard]] SourceLocation Token::getLocation() const {
    return m_location;
}

[[nodiscard]] std::string Token::getLexeme() const {
    return m_lexeme;
}
