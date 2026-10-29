//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//


#pragma once
#include <string>
#include <string_view>

enum class TokenKind {

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,
    BOOLEAN,

    // Keywords
    LET,
    CONST,
    FUNC,
    DEFINE,
    IF,
    ELSE,
    ELSEIF,
    WHILE,
    FOREACH,
    RETURN,
    BREAK,
    CONTINUE,
    STRUCT,
    NULL_LITERAL,
    THROW,
    EXPORT,

    // Delimiters
    LEFT_PAREN,
    RIGHT_PAREN,

    LEFT_BRACE,
    RIGHT_BRACE,

    LEFT_BRACKET,
    RIGHT_BRACKET,

    COMMA,
    DOT,
    COLON,
    SEMICOLON,

    // Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,

    PLUS_PLUS,
    MINUS_MINUS,
    STAR_STAR,
    SLASH_SLASH,

    EQUAL,
    PLUS_EQUAL,
    MINUS_EQUAL,
    STAR_EQUAL,
    SLASH_EQUAL,
    PERCENT_EQUAL,

    EQUAL_EQUAL,
    BANG,
    BANG_EQUAL,

    LESS,
    LESS_EQUAL,

    GREATER,
    GREATER_EQUAL,

    AND,
    OR,

    QUESTION,
    PIPE,
    ARROW,

    // Special
    END_OF_FILE,
};

[[nodiscard]] inline std::string_view to_string(const TokenKind tokenKind) {
    switch (tokenKind) {
        case TokenKind::IDENTIFIER: return "IDENTIFIER";
        case TokenKind::STRING: return "STRING";
        case TokenKind::NUMBER: return "NUMBER";
        case TokenKind::BOOLEAN: return "BOOLEAN";

        case TokenKind::LET: return "LET";
        case TokenKind::CONST: return "CONST";
        case TokenKind::FUNC: return "FUNC";
            case TokenKind::DEFINE: return "DEFINE";
            case TokenKind::IF: return "IF";
            case TokenKind::ELSE: return "ELSE";
            case TokenKind::ELSEIF: return "ELSEIF";
            case TokenKind::WHILE: return "WHILE";
            case TokenKind::FOREACH: return "FOREACH";
            case TokenKind::RETURN: return "RETURN";
            case TokenKind::BREAK: return "BREAK";
            case TokenKind::CONTINUE: return "CONTINUE";
            case TokenKind::STRUCT: return "STRUCT";
            case TokenKind::NULL_LITERAL: return "NULL";
            case TokenKind::THROW: return "THROW";
        case TokenKind::EXPORT: return "EXPORT";

            case TokenKind::LEFT_PAREN: return "LEFT_PAREN";
            case TokenKind::RIGHT_PAREN: return "RIGHT_PAREN";
            case TokenKind::LEFT_BRACE: return "LEFT_BRACE";
            case TokenKind::RIGHT_BRACE: return "RIGHT_BRACE";
            case TokenKind::LEFT_BRACKET: return "LEFT_BRACKET";
            case TokenKind::RIGHT_BRACKET: return "RIGHT_BRACKET";

        case TokenKind::COMMA: return "COMMA";
            case TokenKind::DOT: return "DOT";
            case TokenKind::COLON: return "COLON";
            case TokenKind::SEMICOLON: return "SEMICOLON";

        case TokenKind::PLUS: return "PLUS";
            case TokenKind::MINUS: return "MINUS";
            case TokenKind::STAR: return "STAR";
            case TokenKind::SLASH: return "SLASH";
            case TokenKind::PERCENT: return "PERCENT";

            case TokenKind::PLUS_PLUS: return "PLUS_PLUS";
            case TokenKind::MINUS_MINUS: return "MINUS_MINUS";
            case TokenKind::STAR_STAR: return "STAR_STAR";
            case TokenKind::SLASH_SLASH: return "SLASH_SLASH";

            case TokenKind::EQUAL: return "EQUAL";
            case TokenKind::PLUS_EQUAL: return "PLUS_EQUAL";
            case TokenKind::MINUS_EQUAL: return "MINUS_EQUAL";
            case TokenKind::STAR_EQUAL: return "STAR_EQUAL";
            case TokenKind::SLASH_EQUAL: return "SLASH_EQUAL";
            case TokenKind::PERCENT_EQUAL: return "PERCENT_EQUAL";

            case TokenKind::EQUAL_EQUAL: return "EQUAL_EQUAL";
            case TokenKind::BANG_EQUAL: return "BANG_EQUAL";
            case TokenKind::BANG: return "BANG";

        case TokenKind::LESS: return "LESS";
            case TokenKind::LESS_EQUAL: return "LESS_EQUAL";
            case TokenKind::GREATER_EQUAL: return "GREATER_EQUAL";
            case TokenKind::GREATER: return "GREATER";

            case TokenKind::AND: return "AND";
            case TokenKind::OR: return "OR";

            case TokenKind::QUESTION: return "QUESTION";
            case TokenKind::PIPE: return "PIPE";
        case TokenKind::ARROW: return "ARROW";

            case TokenKind::END_OF_FILE: return "END_OF_FILE";
    }

    return "UNKNOWN";
}