//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <vector>

#include "../ast/Statement.h"
#include "../ast/statements/BlockStatement.h"
#include "../ast/statements/BreakStatement.h"
#include "../ast/statements/ForeachStatement.h"
#include "../ast/statements/IfStatement.h"
#include "../ast/statements/VariableDeclaration.h"
#include "../ast/statements/WhileStatement.h"
#include "../ast/statements/ContinueStatement.h"
#include "../ast/statements/DestructureDeclaration.h"
#include "../ast/statements/FunctionDeclaration.h"
#include "../ast/statements/ReturnStatement.h"
#include "../ast/statements/StructDeclaration.h"
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

    [[nodiscard]] std::shared_ptr<IfStatement> _ifStatement();
    [[nodiscard]] std::shared_ptr<WhileStatement> _whileStatement();
    [[nodiscard]] std::shared_ptr<ForeachStatement> _foreachStatement();
    [[nodiscard]] std::shared_ptr<BreakStatement> _breakStatement();
    [[nodiscard]] std::shared_ptr<ContinueStatement> _continueStatement();
    [[nodiscard]] std::shared_ptr<ReturnStatement> _returnStatement();
    [[nodiscard]] std::shared_ptr<BlockStatement> _blockStatement();

    [[nodiscard]] std::shared_ptr<FunctionDeclaration> _functionDeclaration(const bool &exported = false);
    [[nodiscard]] std::shared_ptr<StructDeclaration> _structDeclaration(const bool &exported = false);

    [[nodiscard]] std::shared_ptr<DestructureDeclaration> _destructureDeclaration(const bool& _mutable, const bool& exported = false);
    [[nodiscard]] static std::vector<VariableExpression> _destructureTargets(const std::shared_ptr<Expression>& pattern);

    [[nodiscard]] std::vector<std::shared_ptr<Statement>> _block();

    [[nodiscard]] std::shared_ptr<Expression> _arrayLiteral();
    [[nodiscard]] std::shared_ptr<Expression> _mapLiteral();
    [[nodiscard]] std::shared_ptr<Expression> _tupleLiteral();

    [[nodiscard]] TypeExpressionPtr _typeAnnotation();
    [[nodiscard]] TypeExpressionPtr _unionTypeAnnotation();
    [[nodiscard]] TypeExpressionPtr _postfixTypeAnnotation();
    [[nodiscard]] TypeExpressionPtr _primaryTypeAnnotation();
    [[nodiscard]] TypeExpressionPtr _parenthesisedTypeAnnotation(const SourceLocation& location);

    void _consumeTypeArgumentsEnd();

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
