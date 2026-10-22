//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "Parser.h"

#include "../ast/expressions/AssignmentExpression.h"
#include "../ast/expressions/BinaryExpression.h"
#include "../ast/expressions/LogicalExpression.h"
#include "../ast/expressions/UnaryExpression.h"
#include "../ast/expressions/VariableExpression.h"
#include "../ast/expressions/literals/BooleanLiteral.h"
#include "../ast/expressions/literals/NumberLiteral.h"
#include "../ast/expressions/literals/StringLiteral.h"
#include "../ast/statements/ExpressionStatement.h"

std::vector<std::shared_ptr<Statement>> Parser::parse() {
    std::vector<std::shared_ptr<Statement>> statements{};

    while (!this->_isAtEnd()) {
        statements.push_back(this->_declaration());
    }
    return statements;
}

std::shared_ptr<Statement> Parser::_declaration() {
    if (this->_match(TokenKind::EXPORT)) {
        return this->_exportDeclaration();
    }
    return this->_varDeclaration();
}

std::shared_ptr<Statement> Parser::_varDeclaration(const bool exported) {
    if (this->_match(TokenKind::LET)) {
        return std::make_unique<VariableDeclaration>(this->_variableDeclaration(true, exported));
    } if (this->_match(TokenKind::CONST)) {
        return std::make_unique<VariableDeclaration>(this->_variableDeclaration(false, exported));
    }
        // } if (this->_match(TokenKind::FUNC)) {
    //     return std::make_unique<Statement>(this->_functionDeclaration(exported));
    // } if (this->_match(TokenKind::STRUCT)) {
    //     return std::make_unique<Statement>(this->_structDeclaration(exported));
    // }
    return this->_statement();
}

std::shared_ptr<Statement> Parser::_statement() {
    // if (this->_match(TokenKind::IF)) { return this->_ifStatement(); }
    // if (this->_match(TokenKind::WHILE)) { return this->_whileStatement(); }
    // if (this->_match(TokenKind::FOREACH)) { return this->_foreachStatement(); }
    // if (this->_match(TokenKind::BREAK)) {
    //     if (!this->m_insideLoop) { throw std::runtime_error("BREAK"); }
    //     return this->_breakStatement();
    // }
    // if (this->_match(TokenKind::CONTINUE)) {
    //     if (!this->m_insideLoop) { throw std::runtime_error("CONTINUE"); }
    //     return this->_continueStatement();
    // }
    // if (this->_match(TokenKind::RETURN)) {
    //     if (!this->m_insideFunction) { throw std::runtime_error("RETURN"); }
    //     return this->_returnStatement();
    // }
    // if (this->_match(TokenKind::THROW)) { return this->_throwStatement(); }
    // if (this->_match(TokenKind::LEFT_BRACE)) { return this->_blockStatement(); }

    return this->_expressionStatement();
}

std::shared_ptr<Statement> Parser::_expressionStatement() {
    const std::shared_ptr<Expression> expression = this->_expression();

    static_cast<void>(this->_consume(TokenKind::SEMICOLON, "Expected ';' after expression"));
    return std::make_unique<ExpressionStatement>(ExpressionStatement(expression->getSourceLocation(), expression));
}

std::shared_ptr<Statement> Parser::_exportDeclaration() {
    if (this->m_insideFunction) { throw std::runtime_error("Cannot use 'export' inside a function."); }
    if (this->m_insideLoop) { throw std::runtime_error("Cannot use 'export' inside a loop."); }
    if (this->_peek().getTokenKind() == TokenKind::LET) { throw std::runtime_error("Cannot export a mutable value."); }

    std::vector valid{TokenKind::CONST, TokenKind::FUNC, TokenKind::STRUCT};
    if (const int count = std::count(valid.begin(), valid.end(), this->_peek().getTokenKind()); count == 0) {
        throw std::runtime_error("Expected a declaration after 'export'.");
    }
    return this->_varDeclaration(true);
}

VariableDeclaration Parser::_variableDeclaration(const bool &_mutable, const bool exported) {
    const Token name = this->_consumeIdentifier();

    static_cast<void>(this->_consume(TokenKind::EQUAL, "Expected '=' after variable name"));
    const std::shared_ptr<Expression> initializer = this->_expression();
    static_cast<void>(this->_consume(TokenKind::SEMICOLON, "Expected ';' after variable declaration"));

    return VariableDeclaration(name.getLocation(), name.getLexeme(), initializer, _mutable, exported);

}

std::shared_ptr<Expression> Parser::_expression() {
    return this->_assignment();
}

std::shared_ptr<Expression> Parser::_assignment() {
    std::shared_ptr<Expression> expression = this->_logicalOr();

    if (this->_match(TokenKind::EQUAL)) {
        const Token& equals = this->_previous();
        std::shared_ptr<Expression> value = this->_assignment();
        return _buildAssignmentTarget(expression, equals.getLocation(), value);
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_buildAssignmentTarget(std::shared_ptr<Expression>& target, const SourceLocation &location, std::shared_ptr<Expression>& value) {
    if (typeid(VariableExpression) == typeid(target)) {
        const auto& expr = static_cast<const VariableExpression&>(*target);
        return std::make_unique<AssignmentExpression>(AssignmentExpression(location, expr.getName(), value));
    }

    throw std::runtime_error{"PARSER::_buildAssignmentTarget"};
}

std::shared_ptr<Expression> Parser::_logicalOr() {
    std::shared_ptr<Expression> expression = this->_logicalAnd();

    while (this->_match(TokenKind::OR)) {
        const Token& op = this->_previous();

        std::shared_ptr<Expression> right = this->_logicalAnd();

        expression = std::make_shared<LogicalExpression>(LogicalExpression(op.getLocation(), expression, op, right));
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_logicalAnd() {
    std::shared_ptr<Expression> expression = this->_equality();

    while (this->_match(TokenKind::AND)) {
        const Token& op = this->_previous();

        std::shared_ptr<Expression> right = this->_equality();

        expression = std::make_shared<LogicalExpression>(LogicalExpression(op.getLocation(), expression, op, right));
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_equality() {
    std::shared_ptr<Expression> expression = this->_comparison();

    while (this->_match(std::vector{TokenKind::EQUAL_EQUAL, TokenKind::BANG_EQUAL})) {
        const Token& op = this->_previous();

        std::shared_ptr<Expression> right = this->_comparison();

        expression = std::make_shared<BinaryExpression>(BinaryExpression(op.getLocation(), expression, op, right));
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_comparison() {
    std::shared_ptr<Expression> expression = this->_addition();

    while (this->_match(std::vector{TokenKind::LESS, TokenKind::LESS_EQUAL, TokenKind::GREATER, TokenKind::GREATER_EQUAL})) {
        const Token& op = this->_previous();

        std::shared_ptr<Expression> right = this->_addition();

        expression = std::make_shared<BinaryExpression>(BinaryExpression(op.getLocation(), expression, op, right));
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_addition() {
    std::shared_ptr<Expression> expression = this->_multiplication();

    while (this->_match(std::vector{TokenKind::PLUS, TokenKind::MINUS})) {
        const Token& op = this->_previous();

        std::shared_ptr<Expression> right = this->_addition();

        expression = std::make_shared<BinaryExpression>(BinaryExpression(op.getLocation(), expression, op, right));
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_multiplication() {
    std::shared_ptr<Expression> expression = this->_unary();

    while (this->_match(std::vector{TokenKind::STAR, TokenKind::SLASH, TokenKind::PERCENT})) {
        const Token& op = this->_previous();

        std::shared_ptr<Expression> right = this->_unary();

        expression = std::make_shared<BinaryExpression>(BinaryExpression(op.getLocation(), expression, op, right));
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_unary() {
    if (this->_match(std::vector{TokenKind::BANG, TokenKind::MINUS})) {
        const Token& op = this->_previous();

        const std::shared_ptr<Expression> right = this->_unary();

        return std::make_shared<UnaryExpression>(UnaryExpression(op.getLocation(), op, right));
    }

    return this->_call();
}

std::shared_ptr<Expression> Parser::_call() {
    std::shared_ptr<Expression> expression = this->_primary();

    return expression;
}

std::shared_ptr<Expression> Parser::_primary() {
    if (this->_match(TokenKind::NUMBER)) {
        return std::make_shared<NumberLiteral>(NumberLiteral(this->_previous()));
    } if (this->_match(TokenKind::STRING)) {
        return std::make_shared<StringLiteral>(StringLiteral(this->_previous()));
    } if (this->_match(TokenKind::IDENTIFIER)) {
        const Token& identifier = this->_previous();
        return std::make_shared<VariableExpression>(VariableExpression(identifier.getLocation(), identifier.getLexeme()));
    } if (this->_match(TokenKind::BOOLEAN)) {
        return std::make_shared<BooleanLiteral>(BooleanLiteral(this->_previous()));
    }
    throw std::runtime_error("Expected expression");
}


Token Parser::_consume(const TokenKind &kind, const std::string &errorMsg) {
    if (this->_check(kind)) {
        return this->_advance();
    }
    throw std::runtime_error(errorMsg);
}

Token Parser::_consumeIdentifier() {
    return this->_consume(TokenKind::IDENTIFIER, "Expected variable name.");
}

Token Parser::_consumeSemicolon() {
    return this->_consume(TokenKind::SEMICOLON, "Expected ';'");
}

Token Parser::_consumeLeftBrace() {
    return this->_consume(TokenKind::LEFT_BRACE, "Expected '{'");
}

Token Parser::_consumeRightBrace() {
    return this->_consume(TokenKind::RIGHT_BRACE, "Expected '}'");
}

Token Parser::_advance() {
    const Token& token = this->m_tokens.at(this->m_current);
    this->m_current++;
    return token;
}

bool Parser::_match(const std::vector<TokenKind>& args) {
    for (const auto& token : args) {
        if (this->_check(token)) {
            static_cast<void>(this->_advance());
            return true;
        }
    }
    return false;
}

bool Parser::_match(const TokenKind &arg) {
    if (this->_check(arg)) {
        static_cast<void>(this->_advance());
        return true;
    }
    return false;
}

Token Parser::_previous() {
    return this->m_tokens.at(this->m_current-1);
}

Token Parser::_peek() {
    return this->m_tokens.at(this->m_current);
}

Token Parser::_peekNext() {
    if (this->m_current + 1 >= this->m_tokens.size()) {
        return this->m_tokens.at(-1);
    }
    return this->m_tokens.at(this->m_current + 1);
}

bool Parser::_check(const TokenKind &kind) {
    return this->_peek().getTokenKind() == kind;
}

bool Parser::_checkNext(const TokenKind &kind) {
    if (this->m_current + 1 > this->m_tokens.size()) {
        return false;
    }
    return this->_peekNext().getTokenKind() == kind;
}

bool Parser::_isAtEnd() {
    return this->_peek().getTokenKind() == TokenKind::END_OF_FILE;
}