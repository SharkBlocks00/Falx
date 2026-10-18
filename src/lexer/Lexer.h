//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//


#pragma once
#include <string>
#include <vector>
#include <map>

#include "../tokens/Token.h"


class Lexer {
private:
    // Variables
    std::string m_source;
    std::size_t m_current{0};
    std::size_t m_line{1};
    std::size_t m_start{0};
    std::size_t m_column{1};
    std::size_t m_startColumn{0};
    std::vector<Token> m_tokens{};
    const std::map<std::string, TokenKind> KEYWORDS;
    std::string m_filename;

    // Methods

    void _scanToken();
    void _string();
    void _number();
    void _identifier();
    [[nodiscard]] bool _match(char c);
    void _addToken(const TokenKind kind) {
        _addToken(kind, std::monostate{});
    }
    void _addToken(TokenKind kind, const Literal& literal);
    [[nodiscard]] char _peek() const;
    [[nodiscard]] char _peekNext() const;
    [[nodiscard]] char _advance();
    [[nodiscard]] bool _isAtEnd() const;

    [[nodiscard]] static std::map<std::string, TokenKind> _initKeywords();
public:
    explicit Lexer(std::string source, std::string filename = "");
    [[nodiscard]] std::vector<Token> lex();
};
