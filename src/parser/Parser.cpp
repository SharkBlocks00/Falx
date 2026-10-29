//
// Created by SharkBlocks00 on 10/20/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "Parser.h"

#include "../ast/expressions/AssignmentExpression.h"
#include "../ast/expressions/BinaryExpression.h"
#include "../ast/expressions/FunctionCallExpression.h"
#include "../ast/expressions/GetExpression.h"
#include "../ast/expressions/IndexExpression.h"
#include "../ast/expressions/IndexSetExpression.h"
#include "../ast/expressions/LogicalExpression.h"
#include "../ast/expressions/NullCoalesceExpression.h"
#include "../ast/expressions/OptionalChainExpression.h"
#include "../ast/expressions/SetExpression.h"
#include "../ast/expressions/UnaryExpression.h"
#include "../ast/expressions/VariableExpression.h"
#include "../ast/expressions/literals/ArrayLiteral.h"
#include "../ast/expressions/literals/BooleanLiteral.h"
#include "../ast/expressions/literals/MapLiteral.h"
#include "../ast/expressions/literals/NullLiteral.h"
#include "../ast/expressions/literals/NumberLiteral.h"
#include "../ast/expressions/literals/StringLiteral.h"
#include "../ast/expressions/literals/TupleLiteral.h"
#include "../ast/statements/ExpressionStatement.h"
#include "../ast/types/expressions/ArrayTypeExpression.h"
#include "../ast/types/expressions/FunctionTypeExpression.h"
#include "../ast/types/expressions/NamedTypeExpression.h"
#include "../ast/types/expressions/NullableTypeExpression.h"
#include "../ast/types/expressions/TupleTypeExpression.h"
#include "../ast/types/expressions/UnionTypeExpression.h"

std::vector<std::shared_ptr<Statement>> Parser::parse() {
    std::vector<std::shared_ptr<Statement>> statements{};

    while (!this->_isAtEnd()) {
        statements.push_back(this->_declaration());
    }
    return statements;
}

TypeExpressionPtr Parser::parseTypeAnnotation() {
    TypeExpressionPtr annotation = this->_typeAnnotation();
    if (!this->_isAtEnd()) {
        throw std::runtime_error("Unexpected " + this->_peek().getLexeme() + " after type annotation");
    }
    return annotation;
}

std::shared_ptr<Statement> Parser::_declaration() {
    if (this->_match(TokenKind::EXPORT)) {
        return this->_exportDeclaration();
    }
    return this->_varDeclaration();
}

std::shared_ptr<Statement> Parser::_varDeclaration(const bool exported) {
    if (this->_match(TokenKind::LET)) {
        return this->_variableDeclaration(true, exported);
    } if (this->_match(TokenKind::CONST)) {
        return this->_variableDeclaration(false, exported);
    }
    if (this->_match(TokenKind::FUNC)) {
        return this->_functionDeclaration(exported);
    } if (this->_match(TokenKind::STRUCT)) {
        return this->_structDeclaration(exported);
    }
    return this->_statement();
}

std::shared_ptr<Statement> Parser::_statement() {
    if (this->_match(TokenKind::IF)) { return this->_ifStatement(); }
    if (this->_match(TokenKind::WHILE)) { return this->_whileStatement(); }
    if (this->_match(TokenKind::FOREACH)) { return this->_foreachStatement(); }
    if (this->_match(TokenKind::BREAK)) {
        if (!this->m_insideLoop) { throw std::runtime_error("BREAK"); }
        return this->_breakStatement();
    }
    if (this->_match(TokenKind::CONTINUE)) {
        if (!this->m_insideLoop) { throw std::runtime_error("CONTINUE"); }
        return this->_continueStatement();
    }
    if (this->_match(TokenKind::RETURN)) {
        if (!this->m_insideFunction) { throw std::runtime_error("RETURN"); }
        return this->_returnStatement();
    }
    // if (this->_match(TokenKind::THROW)) { return this->_throwStatement(); }
    if (this->_match(TokenKind::LEFT_BRACE)) { return this->_blockStatement(); }

    return this->_expressionStatement();
}

TypeExpressionPtr Parser::_typeAnnotation() {
    return this->_unionTypeAnnotation();
}

TypeExpressionPtr Parser::_unionTypeAnnotation() {
    TypeExpressionPtr first = this->_postfixTypeAnnotation();
    std::vector members{first};

    while (this->_match(TokenKind::PIPE)) {
        members.push_back(this->_postfixTypeAnnotation());
    }

    if (members.size() == 1) {
        return first;
    }

    return std::make_shared<UnionTypeExpression>(first->getLocation(), members);
}

TypeExpressionPtr Parser::_postfixTypeAnnotation() {
    TypeExpressionPtr result = this->_primaryTypeAnnotation();

    while (true) {
        if (this->_match(TokenKind::QUESTION)) {
            result = std::make_shared<NullableTypeExpression>(result->getLocation(), result);
        } else if (this->_check(TokenKind::LEFT_BRACKET) && this->_checkNext(TokenKind::RIGHT_BRACKET)) {
            static_cast<void>(this->_advance());
            static_cast<void>(this->_advance());
            result = std::make_shared<ArrayTypeExpression>(result->getLocation(), result);
        } else {
            return result;
        }
    }
}

TypeExpressionPtr Parser::_primaryTypeAnnotation() {
    if (this->_match(TokenKind::LEFT_PAREN)) {
        return this->_parenthesisedTypeAnnotation(this->_previous().getLocation());
    }

    if (this->_match(TokenKind::NULL_LITERAL)) {
        return std::make_shared<NamedTypeExpression>(this->_previous().getLocation(), "null");
    }

    const Token name = this->_consume(TokenKind::IDENTIFIER, "Expected a type name");
    std::string qualifiedName = name.getLexeme();

    while (this->_check(TokenKind::DOT) && this->_checkNext(TokenKind::IDENTIFIER)) {
        static_cast<void>(this->_advance());
        qualifiedName += "." + this->_advance().getLexeme();
    }

    std::vector<TypeExpressionPtr> arguments;
    if (this->_match(TokenKind::LESS)) {
        arguments.push_back(this->_typeAnnotation());
        while (this->_match(TokenKind::COMMA)) {
            arguments.push_back(this->_typeAnnotation());
        }
        this->_consumeTypeArgumentsEnd();
    }

    return std::make_shared<NamedTypeExpression>(name.getLocation(), qualifiedName, arguments);
}

TypeExpressionPtr Parser::_parenthesisedTypeAnnotation(const SourceLocation &location) {
    std::vector<TypeExpressionPtr> elements;
    bool trailingComma = false;

    if (!this->_check(TokenKind::RIGHT_PAREN)) {
        elements.push_back(this->_typeAnnotation());
        while (this->_match(TokenKind::COMMA)) {
            trailingComma = true;
            if (this->_check(TokenKind::RIGHT_PAREN)) { break; }
            elements.push_back(this->_typeAnnotation());
            trailingComma = false;
        }
    }

    static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after type annotation"));

    if (this->_match(TokenKind::ARROW)) {
        return std::make_shared<FunctionTypeExpression>(location, elements, this->_typeAnnotation());
    }

    if (elements.empty()) {
        throw std::runtime_error("Expected '->' after '()' in a function type");
    }

    if (elements.size() == 1 && !trailingComma) {
        return elements.at(0);
    }

    return std::make_shared<TupleTypeExpression>(location, elements);
}

void Parser::_consumeTypeArgumentsEnd() {
    if (this->_match(TokenKind::GREATER)) {
        return;
    }

    if (this->_check(TokenKind::GREATER_EQUAL)) {
        const Token token = this->_peek();
        const auto location = SourceLocation(token.getLocation().getFilename(), token.getLocation().getLine(), token.getLocation().getColumn() + 1);
        this->m_tokens[this->m_current] = Token(TokenKind::EQUAL, "=", std::monostate(), location);
        return;
    }

    throw std::runtime_error("Expected '>' after type arguments");
}

std::shared_ptr<Statement> Parser::_expressionStatement() {
    const std::shared_ptr<Expression> expression = this->_expression();

    static_cast<void>(this->_consume(TokenKind::SEMICOLON, "Expected ';' after expression"));
    return std::make_unique<ExpressionStatement>(expression->getLocation(), expression);
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

std::shared_ptr<Statement> Parser::_variableDeclaration(const bool &_mutable, const bool exported) {
    if (this->_match(TokenKind::LEFT_PAREN)) {
        return this->_destructureDeclaration(_mutable, exported);
    }

    const Token name = this->_consumeIdentifier();
    TypeExpressionPtr typeAnnotation{nullptr};

    if (this->_match(TokenKind::COLON)) {
        typeAnnotation = this->_typeAnnotation();
    }

    static_cast<void>(this->_consume(TokenKind::EQUAL, "Expected '=' after variable name"));
    const std::shared_ptr<Expression> initializer = this->_expression();
    static_cast<void>(this->_consume(TokenKind::SEMICOLON, "Expected ';' after variable declaration"));

    return std::make_shared<VariableDeclaration>(name.getLocation(), name.getLexeme(), initializer, _mutable, exported, typeAnnotation);

}

std::shared_ptr<DestructureDeclaration> Parser::_destructureDeclaration(const bool &_mutable, const bool &exported) {
    const SourceLocation location = this->_previous().getLocation();
    const std::vector<VariableExpression> targets = this->_destructureTargets(this->_tupleLiteral());

    static_cast<void>(this->_consume(TokenKind::EQUAL, "Expected '=' after destructuring pattern"));
    const std::shared_ptr<Expression> initializer = this->_expression();
    static_cast<void>(this->_consume(TokenKind::SEMICOLON, "Expected ';' after variable declaration"));

    return std::make_shared<DestructureDeclaration>(location, targets, initializer, _mutable, exported);
}

std::vector<VariableExpression> Parser::_destructureTargets(
    const std::shared_ptr<Expression>& pattern
) {
    if (!pattern || typeid(*pattern) != typeid(TupleLiteral)) {
        throw std::invalid_argument(
            "A destructuring pattern must be a tuple, use '(a,)' for a single variable"
        );
    }

    auto& tuple = static_cast<TupleLiteral&>(*pattern);

    std::vector<VariableExpression> result;

    for (const auto& element : tuple.getElements()) {
        if (!element || typeid(*element) != typeid(VariableExpression)) {
            throw std::runtime_error(
                "Only variable names can appear in a destructuring pattern"
            );
        }

        result.push_back(
            static_cast<const VariableExpression&>(*element)
        );
    }

    return result;
}

std::shared_ptr<FunctionDeclaration> Parser::_functionDeclaration(const bool &exported) {
    const Token name = this->_consumeIdentifier();

    static_cast<void>(this->_consume(TokenKind::EQUAL, "Expected '=' after function name"));
    static_cast<void>(this->_consume(TokenKind::DEFINE, "Expected 'define' after function"));
    static_cast<void>(this->_consume(TokenKind::LEFT_PAREN, "Expected '(' after function 'define'"));

    std::vector<ParameterDefinition> parameters;

    if (!this->_check(TokenKind::RIGHT_PAREN)) {
        bool defaultCreated = false;
        while (true) {
            const Token parameter = this->_consume(TokenKind::IDENTIFIER, "Expected parameter name");

            TypeExpressionPtr paramTypeAnnotation{nullptr};
            std::shared_ptr<Expression> defaultValue;

            if (this->_match(TokenKind::COLON)) {
                paramTypeAnnotation = this->_typeAnnotation();
            } if (this->_match(TokenKind::EQUAL)) {
                defaultValue = this->_expression();
                defaultCreated = true;
            } else if (defaultCreated) {
                throw std::runtime_error("Invalid default value creation");
            }

            parameters.emplace_back(parameter.getLexeme(), defaultValue, paramTypeAnnotation);
            if (!this->_match(TokenKind::COMMA)) { break; }
        }
    }

    static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after function parameters"));

    TypeExpressionPtr returnTypeAnnotation{nullptr};
    if (this->_match(TokenKind::COLON)) {
        returnTypeAnnotation = this->_typeAnnotation();
    }

    static_cast<void>(this->_consumeLeftBrace());

    bool old = this->m_insideFunction;
    bool inLoop = this->m_insideLoop;

    this->m_insideFunction = true;
    this->m_insideLoop = false;

    std::vector<std::shared_ptr<Statement>> body = this->_block();

    this->m_insideFunction = old;
    this->m_insideLoop = inLoop;
    return std::make_shared<FunctionDeclaration>(name.getLocation(), name.getLexeme(), parameters, body, exported, returnTypeAnnotation);
    }

std::shared_ptr<StructDeclaration> Parser::_structDeclaration(const bool &exported) {
    const Token name = this->_consumeIdentifier();
    static_cast<void>(this->_consumeLeftBrace());
    std::vector<FieldDefinition> fields;
    std::vector<FunctionDeclaration> methods;

    while (!this->_check(TokenKind::RIGHT_BRACE)) {
        if (this->_isAtEnd()) {
            throw std::runtime_error("Unexpected end of input");
        }
        if (this->_peek().getTokenKind() == TokenKind::IDENTIFIER) {
            const Token field = this->_consumeIdentifier();

            TypeExpressionPtr fieldType{nullptr};
            std::shared_ptr<Expression> defaultValue;

            if (this->_match(TokenKind::COLON)) {
                fieldType = this->_typeAnnotation();
            } if (this->_match(TokenKind::EQUAL)) {
                defaultValue = this->_expression();
            }

            static_cast<void>(this->_consume(TokenKind::SEMICOLON, "Expected ';' after struct field"));

            fields.emplace_back(field.getLexeme(), defaultValue, fieldType, field.getLocation());
        } else {
            if (this->_match(TokenKind::FUNC)) {
                auto function = this->_functionDeclaration(false);
                methods.push_back(std::move(*function));
            } else {
                throw std::runtime_error("Expected field or method in struct body");
            }
        }
    }
    static_cast<void>(this->_consumeRightBrace());
    return std::make_shared<StructDeclaration>(StructDeclaration(name.getLocation(), name.getLexeme(), fields, std::move(methods), exported));
}

std::shared_ptr<IfStatement> Parser::_ifStatement() {
    const SourceLocation location = this->_previous().getLocation();

    static_cast<void>(this->_consume(TokenKind::LEFT_PAREN, "Expected '(' after 'if'"));
    std::shared_ptr<Expression> condition = this->_expression();
    static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after if statement condition"));

    static_cast<void>(this->_consumeLeftBrace());
    std::vector<std::shared_ptr<Statement>> body = this->_block();

    std::shared_ptr<Statement> elseBranch{nullptr};

    if (this->_match(TokenKind::ELSEIF)) {
        elseBranch = this->_ifStatement();
    } else if (this->_match(TokenKind::ELSE)) {
        static_cast<void>(this->_consumeLeftBrace());
        elseBranch = std::make_shared<BlockStatement>(location, this->_block());
    }

    return std::make_shared<IfStatement>(location, condition, body, elseBranch);
}

std::shared_ptr<WhileStatement> Parser::_whileStatement() {
    const SourceLocation location = this->_previous().getLocation();

    static_cast<void>(this->_consume(TokenKind::LEFT_PAREN, "Expected '(' after 'while'"));
    std::shared_ptr<Expression> condition = this->_expression();
    static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after while statement condition"));

    static_cast<void>(this->_consumeLeftBrace());
    const bool old = this->m_insideLoop;
    this->m_insideLoop = true;

    std::vector<std::shared_ptr<Statement>> body = this->_block();
    this->m_insideLoop = old;
    return std::make_shared<WhileStatement>(location, condition, body);
}

std::vector<std::shared_ptr<Statement>> Parser::_block() {
    std::vector<std::shared_ptr<Statement>> block;

    while (!this->_check(TokenKind::RIGHT_BRACE) && !this->_isAtEnd()) {
        block.push_back(this->_declaration());
    }

    if (this->_match(TokenKind::RIGHT_BRACE)) {
        return block;
    }

    throw std::runtime_error("Expected '}'");

}

std::shared_ptr<ForeachStatement> Parser::_foreachStatement() {
    const SourceLocation location = this->_previous().getLocation();

    static_cast<void>(this->_consume(TokenKind::LEFT_PAREN, "Expected '(' after foreach statement"));
    const std::string variable = this->_consumeIdentifier().getLexeme();
    static_cast<void>(this->_consume(TokenKind::COLON, "Expected ':' after foreach variable"));

    const std::shared_ptr<Expression> iterable = this->_expression();
    static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after foreach iterable"));
    static_cast<void>(this->_consumeLeftBrace());

    const bool old = this->m_insideLoop;
    this->m_insideLoop = true;
    std::vector<std::shared_ptr<Statement>> body = this->_block();

    this->m_insideLoop = old;
    return std::make_shared<ForeachStatement>(location, variable, iterable, body);
}

std::shared_ptr<ReturnStatement> Parser::_returnStatement() {
    const SourceLocation location = this->_previous().getLocation();

    if (this->_match(TokenKind::SEMICOLON)) {
        return std::make_shared<ReturnStatement>(location, nullptr);
    }
    const std::shared_ptr<Expression> value = this->_expression();
    static_cast<void>(this->_consumeSemicolon());
    return std::make_shared<ReturnStatement>(location, value);
}

std::shared_ptr<BreakStatement> Parser::_breakStatement() {
    const SourceLocation location = this->_previous().getLocation();
    static_cast<void>(this->_consumeSemicolon());
    return std::make_shared<BreakStatement>(location);
}

std::shared_ptr<ContinueStatement> Parser::_continueStatement() {
    const SourceLocation location = this->_previous().getLocation();
    static_cast<void>(this->_consumeSemicolon());
    return std::make_shared<ContinueStatement>(location);
}

std::shared_ptr<BlockStatement> Parser::_blockStatement() {
    const SourceLocation location = this->_previous().getLocation();
    const std::vector<std::shared_ptr<Statement>> block = this->_block();
    return std::make_shared<BlockStatement>(location, block);
}

std::shared_ptr<Expression> Parser::_arrayLiteral() {
    const SourceLocation location = this->_previous().getLocation();

    std::vector<std::shared_ptr<Expression>> array;

    if (!this->_check(TokenKind::RIGHT_BRACKET)) {
        array.push_back(this->_expression());
        while (this->_match(TokenKind::COMMA)) {
            array.push_back(this->_expression());
        }
    }

    static_cast<void>(this->_consume(TokenKind::RIGHT_BRACKET, "Expected ']' after array"));
    return std::make_shared<ArrayLiteral>(location, array);
}

std::shared_ptr<Expression> Parser::_mapLiteral() {
    const SourceLocation location = this->_previous().getLocation();

    std::map<std::string, std::shared_ptr<Expression>> map;

    if (!this->_check(TokenKind::RIGHT_BRACE)) {
        std::string key = this->_consumeIdentifier().getLexeme();
        static_cast<void>(this->_consume(TokenKind::COLON, "Expected ':' to separate map key and value"));
        std::shared_ptr<Expression> value = this->_expression();
        map[key] = value;

        while (this->_match(TokenKind::COMMA)) {
            key = this->_consumeIdentifier().getLexeme();
            static_cast<void>(this->_consume(TokenKind::COLON, "Expected ':' to separate map key and value"));
            value = this->_expression();
            map[key] = value;
        }
    }

    static_cast<void>(this->_consumeRightBrace());
    return std::make_shared<MapLiteral>(location, map);
}

std::shared_ptr<Expression> Parser::_tupleLiteral() {
    const SourceLocation location = this->_previous().getLocation();

    if (this->_match(TokenKind::COMMA)) {
        static_cast<void>(this->_consume(TokenKind::COMMA, "Expected ',' to complete an empty tuple '(,,)'"));
        static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after an empty tuple '(,,)'"));
        std::vector<std::shared_ptr<Expression>> elements{};
        return std::make_shared<TupleLiteral>(location, elements);
    }

    const std::shared_ptr<Expression> first = this->_expression();

    if (!this->_match(TokenKind::COMMA)) {
        static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after an expression"));
        return first;
    }

    std::vector elements{first};

    while (!this->_check(TokenKind::RIGHT_PAREN)) {
        elements.push_back(this->_expression());
        if (!this->_match(TokenKind::COMMA)) { break; }
    }

    static_cast<void>(this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after tuple elements"));
    return std::make_shared<TupleLiteral>(location, elements);

}

std::shared_ptr<Expression> Parser::_expression() {
    return this->_assignment();
}


std::shared_ptr<Expression> Parser::_assignment() {
    std::shared_ptr<Expression> expression = this->_nullCoalescing();

    if (this->_match(TokenKind::EQUAL)) {
        const Token& equals = this->_previous();
        std::shared_ptr<Expression> value = this->_assignment();
        return _buildAssignmentTarget(expression, equals.getLocation(), value);
    }

    for (const auto& [compoundToken, operation] : this->m_compound_assignment_operators) {
        const auto& [baseOp, lexeme] = operation;
        if (this->_match(compoundToken)) {
            const Token operatorToken = this->_previous();
            std::shared_ptr<Expression> rhs = this->_assignment();

            const auto _operator = Token(baseOp, std::string{lexeme}, std::monostate(), operatorToken.getLocation());
            return this->_buildCompoundAssignment(expression, operatorToken.getLocation(), _operator, rhs);
        }
    }

    if (this->_match(std::vector{TokenKind::PLUS_PLUS, TokenKind::MINUS_MINUS})) {
        const Token operatorToken = this->_previous();
        TokenKind baseOp = operatorToken.getTokenKind() == TokenKind::PLUS_PLUS? TokenKind::PLUS : TokenKind::MINUS;
        const std::string lexeme = baseOp == TokenKind::PLUS? "+" : "-";

        const auto _operator = Token(baseOp, lexeme, std::monostate(), operatorToken.getLocation());
        std::shared_ptr<Expression> one = std::make_shared<NumberLiteral>(Token(TokenKind::NUMBER, "1", 1, operatorToken.getLocation()));

        return this->_buildCompoundAssignment(expression, operatorToken.getLocation(), _operator, one);
    }

    return expression;
}

std::shared_ptr<Expression> Parser::_buildAssignmentTarget(std::shared_ptr<Expression>& target, const SourceLocation &location, std::shared_ptr<Expression>& value) const {
    if (const auto* expr = dynamic_cast<VariableExpression*>(target.get())) {
        return std::make_shared<AssignmentExpression>(location, expr->getName(), value);
    } if (const auto* expr = dynamic_cast<GetExpression*>(target.get())) {
        return std::make_shared<SetExpression>(location, expr->getObject(), expr->getProperty(), value);
    } if (const auto* expr = dynamic_cast<IndexExpression*>(target.get())) {
        return std::make_shared<IndexSetExpression>(location, expr->getObject(), expr->getIndex(), value);
    } if (dynamic_cast<TupleLiteral*>(target.get())) {
        return std::make_shared<DestructureAssignment>(location, this->_destructureTargets(target), value);
    }

    throw std::runtime_error{"PARSER::_buildAssignmentTarget"};
}

std::shared_ptr<Expression> Parser::_buildCompoundAssignment(std::shared_ptr<Expression> &target, const SourceLocation &location, const Token &_operator, std::shared_ptr<Expression> &rhs) {
    if (const auto* expr = dynamic_cast<VariableExpression*>(target.get())) {
        std::shared_ptr<Expression> combined = std::make_shared<BinaryExpression>(location, target, _operator, rhs);
        return std::make_shared<AssignmentExpression>(location, expr->getName(), combined);
    } if (const auto* expr = dynamic_cast<GetExpression*>(target.get())) {
        return std::make_shared<SetExpression>(location, expr->getObject(), expr->getProperty(), rhs, _operator);
    } if (const auto* expr = dynamic_cast<IndexExpression*>(target.get())) {
        return std::make_shared<IndexSetExpression>(location, expr->getObject(), expr->getIndex(), rhs, _operator);
    }
    throw std::runtime_error{"PARSER::_buildCompoundAssignment"};
}

std::shared_ptr<Expression> Parser::_nullCoalescing() {
    std::shared_ptr<Expression> expression = this->_logicalOr();

    if (this->_match(TokenKind::QUESTION_QUESTION)) {
        const Token _operator = this->_previous();
        std::shared_ptr<Expression> right = this->_nullCoalescing();
        return std::make_shared<NullCoalesceExpression>(_operator.getLocation(), expression, right);
    }

    return expression;
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

        std::shared_ptr<Expression> right = this->_multiplication();

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
    bool optionalChain{false};

    while (true) {
        if (this->_match(TokenKind::LEFT_PAREN)) {
            expression = this->_parseCallArguments(expression);
        } else if (this->_match(TokenKind::DOT)) {
            const Token& name = this->_consume(TokenKind::IDENTIFIER, "Expected property name");
            expression = std::make_shared<GetExpression>(name.getLocation(), expression, name.getLexeme());
        } else if (this->_match(TokenKind::QUESTION_DOT)) {
            const Token& name = this->_consume(TokenKind::IDENTIFIER, "Expected property name after '?.'");
            expression = std::make_shared<GetExpression>(name.getLocation(), expression, name.getLexeme(), true);
            optionalChain = true;
        } else if (this->_check(TokenKind::QUESTION) && this->_checkNext(TokenKind::LEFT_BRACKET)) {
            static_cast<void>(this->_advance());
            static_cast<void>(this->_advance());
            std::shared_ptr<Expression> index = this->_expression();
            static_cast<void>(this->_consume(TokenKind::RIGHT_BRACKET, "Expected ']'"));
            expression = std::make_shared<IndexExpression>(expression->getLocation(), expression, index, true);
            optionalChain = true;
        } else if (this->_match(TokenKind::LEFT_BRACKET)) {
            std::shared_ptr<Expression> index = this->_expression();
            static_cast<void>(this->_consume(TokenKind::RIGHT_BRACKET, "Expected ']'"));
            expression = std::make_shared<IndexExpression>(expression->getLocation(), expression, index);
        } else { break; }
    }

    if (optionalChain) {
        expression = std::make_shared<OptionalChainExpression>(expression->getLocation(), expression);
    }
    return expression;
}

std::shared_ptr<Expression> Parser::_parseCallArguments(std::shared_ptr<Expression> &callee) {
    std::vector<std::shared_ptr<Expression>> arguments;

    if (!this->_check(TokenKind::RIGHT_PAREN)) {
        arguments.push_back(this->_expression());

        while (this->_match(TokenKind::COMMA)) {
            arguments.push_back(this->_expression());
        }
    }
    const Token& closing = this->_consume(TokenKind::RIGHT_PAREN, "Expected ')' after arguments");

    return std::make_shared<FunctionCallExpression>(closing.getLocation(), callee, arguments);
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
    } if (this->_match(TokenKind::LEFT_PAREN)) {
        return this->_tupleLiteral();
    } if (this->_match(TokenKind::LEFT_BRACE)) {
        return this->_mapLiteral();
    } if (this->_match(TokenKind::END_OF_FILE)) {
        throw std::runtime_error("Expected expression, found 'EOF'");
    } if (this->_match(TokenKind::NULL_LITERAL)) {
        return std::make_shared<NullLiteral>(this->_previous());
    } if (this->_match(TokenKind::SEMICOLON)) {
        throw std::runtime_error("Expected expression, found ';'");
    } if (this->_match(TokenKind::ELSEIF)) {
        throw std::runtime_error("Cannot call 'elseif' without a valid if");
    } if (this->_match(TokenKind::ELSE)) {
        throw std::runtime_error("Cannot call 'else' without a valid if");
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
        return this->m_tokens.back();
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