//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <vector>

#include "../ast/Statement.h"
#include "../ast/statements/VariableDeclaration.h"
#include "../tokens/Token.h"

class Parser {
private:
    std::vector<Token> m_tokens;
    int m_current{0};
    bool m_insideFunction{false};
    bool m_insideLoop{false};



    [[nodiscard]] std::shared_ptr<Statement> _declaration();
    [[nodiscard]] std::shared_ptr<Statement> _varDeclaration(bool exported=false);
    [[nodiscard]] std::shared_ptr<Expression> _expression();
    [[nodiscard]] std::shared_ptr<Expression> _assignment();

    [[nodiscard]] static std::shared_ptr<Expression> _buildAssignmentTarget(std::shared_ptr<Expression> &target, const SourceLocation &location, std::shared_ptr<Expression> &value);

    [[nodiscard]] std::shared_ptr<Expression> _logicalOr();
    [[nodiscard]] std::shared_ptr<Expression> _logicalAnd();
    [[nodiscard]] std::shared_ptr<Expression> _equality();
    [[nodiscard]] std::shared_ptr<Expression> _comparison();
    [[nodiscard]] std::shared_ptr<Expression> _addition();
    [[nodiscard]] std::shared_ptr<Expression> _multiplication();
    [[nodiscard]] std::shared_ptr<Expression> _unary();
    [[nodiscard]] std::shared_ptr<Expression> _call();
    [[nodiscard]] std::shared_ptr<Expression> _primary();


    [[nodiscard]] std::shared_ptr<Statement> _statement();
    [[nodiscard]] std::shared_ptr<Statement> _expressionStatement();
    [[nodiscard]] std::shared_ptr<Statement> _exportDeclaration();

    [[nodiscard]] VariableDeclaration _variableDeclaration(const bool &_mutable = false, bool exported = false);

    [[nodiscard]] bool _isAtEnd();
    [[nodiscard]] Token _advance();

    [[nodiscard]] Token _consume(const TokenKind& kind, const std::string& errorMsg);
    [[nodiscard]] Token _consumeIdentifier();
    [[nodiscard]] Token _consumeSemicolon();
    [[nodiscard]] Token _consumeLeftBrace();
    [[nodiscard]] Token _consumeRightBrace();

    [[nodiscard]] bool _match(const std::vector<TokenKind>& args);
    [[nodiscard]] bool _match(const TokenKind& arg);
    [[nodiscard]] bool _checkNext(const TokenKind& kind);
    [[nodiscard]] bool _check(const TokenKind& kind);
    [[nodiscard]] Token _previous();
    [[nodiscard]] Token _peekNext();
    [[nodiscard]] Token _peek();
public:
    explicit Parser(const std::vector<Token> &tokens) : m_tokens{tokens} {}

    [[nodiscard]] std::vector<std::shared_ptr<Statement>> parse();
};
